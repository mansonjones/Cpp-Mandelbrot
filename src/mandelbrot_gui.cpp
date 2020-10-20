#ifndef WX_PRECOMP
#       include <wx/wx.h>
#endif

#include "mandelbrot_gui.h"
#include "mandelbrot.h"

#include "BufferEffects.h"
#include "Constants.h"
#include "ImageIO.h"
#include "ImageBuffer.h"
#include "ImageIO_PPM.h"

#include <wx/sizer.h>
#include <wx/utils.h>
#include <wx/wx.h>

#include <iostream>
#include <string>
#include <future>

#include <fstream>

// wxWidgets APP
IMPLEMENT_APP(MandelbrotApp)

BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
        EVT_MENU(MENU_New, MainFrame::NewFile)
        EVT_MENU(MENU_Open, MainFrame::OpenFile)
        EVT_MENU(MENU_Close, MainFrame::CloseFile)
        EVT_MENU(MENU_Save, MainFrame::SaveFile)
        EVT_MENU(MENU_SaveAs, MainFrame::SaveFileAs)
        EVT_MENU(MENU_Quit, MainFrame::Quit)
        EVT_SLIDER(SLIDER_X, MainFrame::SliderX)
        EVT_SLIDER(SLIDER_Y, MainFrame::SliderY)
        EVT_SLIDER(SCALE, MainFrame::Scale)
END_EVENT_TABLE()

bool MandelbrotApp::OnInit()
{
    // make sure to call this first
    wxInitAllImageHandlers();
    
    _frame = new MainFrame(wxT("Mandelbrot Viewer"), wxPoint(50,50), wxSize(500,500));
    _frame->Show(TRUE);
    SetTopWindow(_frame);
    
    return true;
} 
    
MainFrame::MainFrame(const wxString& title, const wxPoint& position, const wxSize& size)
: wxFrame((wxFrame *) NULL, wxID_ANY, title, position, size)
{
       CreateStatusBar(2);

        _mainMenu = new wxMenuBar();
        wxMenu *FileMenu = new wxMenu();

        FileMenu->Append(MENU_New,
                "&New", "Create a new file");
        
        FileMenu->AppendSeparator();
        FileMenu->Append(MENU_Open,
                "&Open", "Open an existing file");
        FileMenu->Append(MENU_Close,
                "&Close", "Close the current document");

        FileMenu->AppendSeparator();
        FileMenu->Append(MENU_Save,
                "&Save", "Save the current document");
        FileMenu->Append(MENU_SaveAs,
                "Save &As", "Save the current document under a new file  name");

        FileMenu->AppendSeparator();
        FileMenu->Append(MENU_Quit,
                "&Quit", "Quit the editor");

        _mainMenu->Append(FileMenu, "&File");
        
        SetMenuBar(_mainMenu);



        _mandelbrotPanel = new MandelbrotPanel( this, wxT("../images/cassini.jpg"), wxBITMAP_TYPE_JPEG); 

        _currentDocPath = ::wxGetCwd();
        _currentFileName = wxString(Constants::defaultFileName);
        SetTitle(_currentFileName);

        _sliderX = new wxSlider(this, SLIDER_X, 0, Constants::sliderXMinimum, Constants::sliderXMaximum, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
        _sliderX->SetBackgroundColour(wxColour(77,77,77));
        _sliderY = new wxSlider(this, SLIDER_Y, 0, Constants::sliderYMinimum, Constants::sliderYMaximum, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
        _sliderY->SetBackgroundColour(wxColour(77,77,77));

        // _sliderX->SetLineSize(5);

        _scale = new wxSlider(this, SCALE, Constants::scaleDefault, Constants::scaleMinimum, Constants::scaleMaximum, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
        _scale->SetBackgroundColour(wxColour(77,77,77));

        wxBoxSizer *vBoxSizer = new wxBoxSizer(wxVERTICAL);
        vBoxSizer->Add(_mandelbrotPanel, 1, wxEXPAND);
        wxBoxSizer *sliderXBoxSizer = new wxBoxSizer(wxHORIZONTAL);
        
        sliderXBoxSizer->Add(new wxStaticText(this,wxID_ANY,_("y Offset ")),0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
        sliderXBoxSizer->Add(_sliderX);
        wxBoxSizer *sliderYBoxSizer = new wxBoxSizer(wxHORIZONTAL);
        
        sliderYBoxSizer->Add(new wxStaticText(this,wxID_ANY,_("x Offset ")),0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
        sliderYBoxSizer->Add(_sliderY);
        
        // vBoxSizer->Add(_sliderY);
        wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxHORIZONTAL);
        scaleBoxSizer->Add(new wxStaticText(this,wxID_ANY,_("Scale")),0,wxALIGN_LEFT /* |wxALIGN_CENTER_VERTICAL */);
        scaleBoxSizer->Add(_scale);

        vBoxSizer->Add(sliderXBoxSizer);
        vBoxSizer->Add(sliderYBoxSizer);
        vBoxSizer->Add(scaleBoxSizer);
        this->SetBackgroundColour(wxColour(77,77,77));

        this->SetSizer(vBoxSizer);

        this->SetStatusText(wxString("Elapsed time : 0:00"));



    // Maximize();  // Maximize the window

}

void MainFrame::NewFile(wxCommandEvent& WXUNUSED(event))
{
    // Clear the panel display
    // MainEditBox->Clear();
    // reset the path of our current open file
    _currentDocPath = ::wxGetCwd();

   wxFileDialog *NewFileDialog = new wxFileDialog(
        this, "Define New File ", wxEmptyString, wxEmptyString,
        "Image Files files (*.ppm)|*.ppm",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

    // Creates a Save Dialog with ppm file type
    if (NewFileDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
    {
        _currentDocPath = NewFileDialog->GetPath();

        SetTitle(wxString("Edit - ") << NewFileDialog->GetFilename());
        _currentFileName = NewFileDialog->GetFilename();

    }

    // Clean up after ourselves
    NewFileDialog->Destroy();
}

void MainFrame::OpenFile(wxCommandEvent& WXUNUSED(event))
{
    
    wxFileDialog *OpenDialog = new wxFileDialog(
            this, "Choose a file to open", "" /* wxEmptyString */, "" /* wxEmptyString */,
            "image files (*.ppm)|*.ppm",
            wxFD_OPEN, wxDefaultPosition);

    // Creates a "open file" dialog with 1 file types
    if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
    {
        // Sets our current document to the file the user selected
        // MainEditBox->LoadFile(CurrentDocPath); //Opens that file
        // Set the Title to reflect the  file open:
        _currentDocPath = OpenDialog->GetPath();
        _currentFileName = OpenDialog->GetFilename();

        SetTitle(wxString("Edit - ") << _currentFileName);

        FileType fileType = PPM;
            
        ImageBuffer<unsigned char> imageBuffer = readFile(PPM, std::string(_currentDocPath.mb_str()));
            
        
            
        _mandelbrotPanel->moveImageBufferHere(imageBuffer);
    }
    OpenDialog->Destroy();

}

void MainFrame::CloseFile(wxCommandEvent& WXUNUSED(event))
{
    _currentDocPath = ::wxGetCwd();
    // Set the Title to reflect the file openha
    std::string fName = "temp_close_file.ppm";
    getMandelbrotPanel()->getMandelbrotPointer()->write(PPM, std::string(_currentFileName.mb_str()));

    SetTitle("Edit - untitled *");
}

void MainFrame::SaveFile(wxCommandEvent& WXUNUSED(event))
{
    // In this case the file is saved using the function object for file Save

    // Be sure to avoid C++'s most vexing parse.
    ImageBuffer<unsigned char> imageBuffer = getMandelbrotPanel()->getImageBuffer();

    std::string fullyQualified = std::string(_currentDocPath.mb_str()) + "/" + std::string(_currentFileName.mb_str());
    std::thread thread1{ SaveJob(PPM, fullyQualified, imageBuffer) };

    thread1.join();

}

void MainFrame::SaveFileAs(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog *SaveDialog = new wxFileDialog(
        this, "Save File As _?", wxEmptyString, wxEmptyString,
        "Image Files files (*.ppm)|*.ppm",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

    // Creates a Save Dialog with 4 file types
    if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
    {
        _currentDocPath = SaveDialog->GetPath();
        // set the path of our current document to the file the user chose to save under
        // MainEditBox->SaveFile(CurrentDocPath); // Save the file to the selected path
        // Set the Title to reflect the file open
        SetTitle(wxString("Edit - ") << SaveDialog->GetFilename());
        _currentFileName = SaveDialog->GetFilename();

        std::cout << " _currentDocPath = " << _currentDocPath << std::endl;

        std::shared_ptr<SaveJob> saveJob = std::make_shared<SaveJob>();
        ImageBuffer<unsigned char> imageBuffer = getMandelbrotPanel()->getImageBuffer();
        std::string fullyQualified = std::string(_currentDocPath.mb_str());
        saveJob->setFileName(fullyQualified);
        saveJob->setFileType(PPM);
        saveJob->setImageBuffer(imageBuffer);

        std::promise<std::string> promise;
        std::future<std::string> future = promise.get_future();

        std::thread thread1(&MainFrame::writeFile, this, std::move(promise), saveJob);

        std::string result = future.get();
        // Thread barrier
        thread1.join();
    
    }

    // Clean up after ourselves
    SaveDialog->Destroy();
}

void MainFrame::Quit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE); // Close the window
}

void MainFrame::SliderX(wxCommandEvent& event)
{
    auto xOffset = _sliderX->GetValue();
    _mandelbrotPanel->getMandelbrotPointer()->setOffsets(xOffset, 0);
    _mandelbrotPanel->recomputeMandelbrot();
}


void MainFrame::SliderY(wxCommandEvent& event)
{
    auto yOffset = _sliderY->GetValue();
    _mandelbrotPanel->getMandelbrotPointer()->setOffsets(0, yOffset);
    _mandelbrotPanel->recomputeMandelbrot();
}

void MainFrame::Scale(wxCommandEvent& event)
{
    auto scale = _scale->GetValue()/1000.0;
    _mandelbrotPanel->getMandelbrotPointer()->setScale(scale);
    _mandelbrotPanel->recomputeMandelbrot();
}

// TODO: Move this function into the IO class
ImageBuffer<unsigned char> MainFrame::readFile(FileType type, std::string fileName)
{
    std::ifstream fileStream(fileName);
    std::string lineBuffer;

   if (fileStream.is_open())
   {
      // read first line
      std::getline(fileStream, lineBuffer);
      if (lineBuffer != "P3") 
      {
         // std::cout << " error! " << std::endl; 
      }

      // read second line
      std::getline(fileStream,lineBuffer);
      std::istringstream lineStream(lineBuffer);
      int width, height, bitDepth;
      lineStream >> width >> height >> bitDepth;
      ImageBuffer<unsigned char> imageBuffer(width, height);
      int red, green, blue;
      for (int i = 0; i < width; ++i) {
         for (int j = 0; j < height; ++j) {
            std::getline(fileStream,lineBuffer);
            // std::istringstream lineStream2(lineBuffer);
            lineStream = std::istringstream(lineBuffer);
            lineStream >> red >> green >> blue;
            imageBuffer.setRed(i,j,static_cast<unsigned char>(red));
            imageBuffer.setGreen(i,j,static_cast<unsigned char>(green));
            imageBuffer.setBlue(i,j,static_cast<unsigned char>(blue));
         }
      }
      fileStream.close();
      return imageBuffer;
   } else {
        ImageBuffer<unsigned char> imageBuffer;
        return imageBuffer;
    }

}

/*
void MainFrame::writeFile(FileType type, std::string fileName, ImageBuffer<unsigned char> imageBuffer)
{
    // Create promise and future
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();

    // Start the thre
}
*/

void MainFrame::writeFile(std::promise<std::string> &&promise, std::shared_ptr<SaveJob> saveJob)
{
    saveJob->write();
    promise.set_value(saveJob->getFileName());
}