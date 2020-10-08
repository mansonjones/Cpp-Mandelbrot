#ifndef WX_PRECOMP
#       include <wx/wx.h>
#endif

#include "mandelbrot_gui.h"
#include "mandelbrot.h"

#include "AutoSave.h"
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


BEGIN_EVENT_TABLE(MandelbrotPanel, wxPanel)
// some useful events
/*
 EVT_MOTION(MandelbrotPanel::mouseMoved)
 EVT_LEFT_DOWN(MandelbrotPanel::mouseDown)
 EVT_LEFT_UP(MandelbrotPanel::mouseReleased)
 EVT_RIGHT_DOWN(MandelbrotPanel::rightClick)
 EVT_LEAVE_WINDOW(MandelbrotPanel::mouseLeftWindow)
 EVT_KEY_DOWN(MandelbrotPanel::keyPressed)
 EVT_KEY_UP(MandelbrotPanel::keyReleased)
 EVT_MOUSEWHEEL(MandelbrotPanel::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(MandelbrotPanel::paintEvent)
//Size event
EVT_SIZE(MandelbrotPanel::OnSize)
END_EVENT_TABLE()


// some useful events
/*
 void MandelbrotPanel::mouseMoved(wxMouseEvent& event) {}
 void MandelbrotPanel::mouseDown(wxMouseEvent& event) {}
 void MandelbrotPanel::mouseWheelMoved(wxMouseEvent& event) {}
 void MandelbrotPanel::mouseReleased(wxMouseEvent& event) {}
 void MandelbrotPanel::rightClick(wxMouseEvent& event) {}
 void MandelbrotPanel::mouseLeftWindow(wxMouseEvent& event) {}
 void MandelbrotPanel::keyPressed(wxKeyEvent& event) {}
 void MandelbrotPanel::keyReleased(wxKeyEvent& event) {}
 */

MandelbrotPanel::MandelbrotPanel(wxFrame* parent, wxString file, wxBitmapType format) :
wxPanel(parent)
{
    // Write raw data to a wxImage
    // Eventually convert to wxImage
    const int width = Constants::panelWidth;
    const int height = Constants::panelHeight;

    _mandelbrotPointer = std::make_unique<Mandelbrot>();
    _mandelbrotPointer->setOffsets(0,0);
    _mandelbrotPointer->setScale(1);

    // _autoSave = new AutoSave(this);
    // _autoSave->runTimerOnThread();
    // _autoSave->runMonitorOnThread();
    // _autoSave->saveJobPollingLoop();
    
    _imageBuffer = ImageBuffer<unsigned char>(width, height);
    _mandelbrotPointer->render(_imageBuffer);   
    // For testing
    // BufferEffects::setColor(yellow, _imageBuffer);
    size_t bufferSize = _imageBuffer.getBufferSize();
    unsigned char *rawBuffer = new unsigned char[bufferSize];
    rawBuffer = _imageBuffer.getBuffer();
    // 
    bool imageCreationSuccess = image.Create( width, height, rawBuffer);
    if (!imageCreationSuccess) {
        std::cerr << " image creation did not succeed " << std::endl;
    }
  // FileType fileType = PPM;
   // std::string fileName = "output2.ppm";
    
   // ImageIO *imageIO = ImageIO::getImageWriter(fileType, fileName, mandelbrotPointer->getImageBuffer());
    w = width;
    h = height;
}

MandelbrotPanel::~MandelbrotPanel()
{ 
    std::cout << " destructor " << std::endl; 
    std::cout << " message queue size = " << _autoSave->messageQueueSize() << std::endl;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void MandelbrotPanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void MandelbrotPanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void MandelbrotPanel::render(wxDC&  dc)
{
    int neww, newh;
    dc.GetSize( &neww, &newh );
    if( neww != w || newh != h )
    {
        resized = wxBitmap( image.Scale( neww, newh /* ,  wxIMAGE_QUALITY_HIGH */ ) );
        w = neww;
        h = newh;
        dc.DrawBitmap( resized, 0, 0, false );
    }else{
        resized = wxBitmap( image );
        dc.DrawBitmap( resized, 0, 0, false );
    }
}

/*
 * Here we call refresh to tell the panel to draw itself again.
 * So when the user resizes the image panel the image should be resized too.
 */
void MandelbrotPanel::OnSize(wxSizeEvent& event){
    Refresh();
    //skip the event.
    event.Skip();
}

void MandelbrotPanel::moveImageBufferHere(ImageBuffer<unsigned char> imageBuffer)
{
    w = _imageBuffer.getWidth();
    h = _imageBuffer.getHeight();

    _imageBuffer = std::move(imageBuffer);

    size_t bufferSize = _imageBuffer.getBufferSize();
    unsigned char *buffer = new unsigned char[bufferSize];
    buffer = _imageBuffer.getBuffer();
    // 
    bool imageCreationSuccess = image.Create( _imageBuffer.getWidth(), _imageBuffer.getHeight(), buffer);
    if (!imageCreationSuccess) {
        std::cerr << " image creation did not succeed " << std::endl;
    }
 
    Refresh();
}

void MandelbrotPanel::debug()
{
    std::cout << " width = " << _imageBuffer.getWidth() << std::endl;
    std::cout << " height = " << _imageBuffer.getHeight() << std::endl;
    int counter = 0;
    for (int i = 0; i < _imageBuffer.getWidth(); i++) {
        for (int j = 0; j < _imageBuffer.getHeight(); j++) {
            std::cout << counter << " " << int(_imageBuffer.getRed(i,j)) << " " << int(_imageBuffer.getGreen(i,j)) << " " << int(_imageBuffer.getBlue(i,j)) << std::endl;
            counter++;
        }
    }
    
}

ImageBuffer<unsigned char> MandelbrotPanel::getImageBuffer() 
{ 
    return _imageBuffer; 
}

void MandelbrotPanel::recomputeMandelbrot()
{
    int width = _imageBuffer.getWidth();
    int height = _imageBuffer.getHeight();

     _mandelbrotPointer->render(_imageBuffer);   
    // For testing
    // BufferEffects::setColor(yellow, _imageBuffer);
    size_t bufferSize = _imageBuffer.getBufferSize();
    unsigned char *rawBuffer = new unsigned char[bufferSize];
    rawBuffer = _imageBuffer.getBuffer();
    // 
    bool imageCreationSuccess = image.Create( width, height, rawBuffer);
    if (!imageCreationSuccess) {
        std::cerr << " image creation did not succeed " << std::endl;
    }
    Refresh();
}
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
        vBoxSizer->Add(_sliderX);
        vBoxSizer->Add(_sliderY);
        vBoxSizer->Add(_scale);

        this->SetBackgroundColour(wxColour(77,77,77));

        this->SetSizer(vBoxSizer);



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
            
        // Diagnostic
        // BufferEffects::setColor(cyan, imageBuffer);
        
            // Use move semantics to copy the image buffer into the MandelbrotPanel class
            _mandelbrotPanel->moveImageBufferHere(imageBuffer);
    }
    OpenDialog->Destroy();

}

void MainFrame::CloseFile(wxCommandEvent& WXUNUSED(event))
{
    // Clear the Text Box
    // MainEditBox->Clear();
    // Reset the current File being edited
    _currentDocPath = ::wxGetCwd();
    // Set the Title to reflect the file openha
    // TODO: What should be the behavior here?  Save the file and reset the
    // name to unknown?
    std::string fName = "temp_close_file.ppm";
    getMandelbrotPanel()->getMandelbrotPointer()->write(PPM, std::string(_currentFileName.mb_str()));

    SetTitle("Edit - untitled *");
}

void MainFrame::SaveFile(wxCommandEvent& WXUNUSED(event))
{
    // Save to the already-set path for the document
    // getMandelbrotPanel()->getMandelbrotPointer()->write(PPM, std::string(_currentFileName.mb_str()));
 
    // Create promise and future
    // std::promise<std::string> promise;
    // std::future<std::string> future = promise.get_future();

    // Start the thread and pass the promise as an argument
    // 
    ImageBuffer<unsigned char> imageBuffer(200,200);

    BufferEffects::setColor(green, imageBuffer);

    std::shared_ptr<SaveJob> saveJob(new SaveJob);
    // SaveJob saveJob;
    std::cout << " path " << _currentDocPath << std::endl;
    std::cout << " file " << _currentFileName << std::endl;

    std::string fullyQualified = std::string(_currentDocPath.mb_str()) + "/" + std::string(_currentFileName.mb_str());
    std::cout << "fully qualified " << fullyQualified << std::endl;
    saveJob->setFileName(fullyQualified);
    saveJob->setFileType(PPM);
    saveJob->setImageBuffer(imageBuffer);
    saveJob->write();
    _saveJobs.push_back(saveJob);
    // Create a thread and call the write function
    // _saveThread = std::thread(&SaveJob::write, saveJob);
    // _saveThread.join();
    // std::thread t(&)
    // saveJob.write();
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // std::thread t(&MainFrame::writeFile, std::move(promise), PPM, _currentFileName, imageBuffer);
    // t.join();

    // MainEditBox->SaveFile(CurrentDocPath);
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

        getMandelbrotPanel()->getMandelbrotPointer()->write(PPM, std::string(_currentFileName.mb_str()));
        // Perform the file write asynchronously.
        // I want to post a message in the title bar when the file write is done
        // std::promise<std::string> promise;
        // std::future<std::string> future = promise.get_future();

        // start the thread and pass promise as an argument
        // std::string tempFileName("tempFilename.ppm");
        // ImageBuffer<unsigned char> tempImageBuffer( 300, 300);
        // std::thread t(ImageIO_PPM::writeImage, std::move(promise), tempFileName, tempImageBuffer);

        // retrieve return message via future and print to console
        // std::string returnMessage = future.get();
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
    std::cout << "slider x " << xOffset << std::endl;
    _mandelbrotPanel->getMandelbrotPointer()->setOffsets(xOffset, 0);
    _mandelbrotPanel->recomputeMandelbrot();
}


void MainFrame::SliderY(wxCommandEvent& event)
{
    auto yOffset = _sliderY->GetValue();
    std::cout << "slider y " << yOffset << std::endl;
    _mandelbrotPanel->getMandelbrotPointer()->setOffsets(0, yOffset);
    _mandelbrotPanel->recomputeMandelbrot();
}

void MainFrame::Scale(wxCommandEvent& event)
{
    auto scale = _scale->GetValue()/1000.0;
    std::cout << " scale " << scale << std::endl;
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

void MainFrame::writeFile(FileType type, std::string fileName, ImageBuffer<unsigned char> imageBuffer)
{
    // Create promise and future
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();

    // Start the thre
}