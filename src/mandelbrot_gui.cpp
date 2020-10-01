#ifndef WX_PRECOMP
#       include <wx/wx.h>
#endif

#include "mandelbrot_gui.h"
#include "mandelbrot.h"

#include "AutoSave.h"
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
    int width = 400; 
    int height = 400;

    // AutoSave *autoSave = new AutoSave();
    // autoSave->runScheduler();
    // autoSave->waitForAutoSave();
    // Note that the ImageBuffer is managed using move semantics
    // ImageBuffer<unsigned char> imageBuffer2(width, height);

    // unsigned char *buffer = new unsigned char[width*height*3];
//    unsigned char *buffer = new unsigned char[width*height*3];

//    for (int i = 0; i < width*height*3; i++) {
//       buffer[i] = 128;
//    }

    _mandelbrotPointer = std::make_shared<Mandelbrot>(width, height);
    _mandelbrotPointer->compute();
    // _mandelbrotPointer = new Mandelbrot(width, height);
    // _mandelbrotPointer->moveImageBufferHere(std::move(imageBuffer2));
    // _mandelbrotPointer->compute();
      _autoSave = new AutoSave(_mandelbrotPointer.get());
    _autoSave->runTimerOnThread();
    _autoSave->runMonitorOnThread();

    std::cout << "debug 3" << std::endl;
    // ImageBuffer<unsigned char> testBuffer = _mandelbrotPointer->getImageBuffer2();

    // std::cout << " Debug Mandelbrot " << std::endl;
    // std::cout << " width " << testBuffer.getWidth() << std::endl;
    // std::cout << " height " << testBuffer.getHeight() << std::endl;
    
    unsigned char *buffer = _mandelbrotPointer->getBuffer();
    // i=diagnostic
    /*
    for (int i = 0; i < 400*400; i = i+3) {
        buffer[i] = (unsigned char)0;
    }
    */
    image2.Create( width, height, buffer);
    
  // FileType fileType = PPM;
   // std::string fileName = "output2.ppm";
    
   // ImageIO *imageIO = ImageIO::getImageWriter(fileType, fileName, mandelbrotPointer->getImageBuffer());
   // imageIO->write();
    
    // load the file... ideally add a check to see if loading was successful
    // image.LoadFile(file, format);
    // w = -1;
    // h = -1;
    w = width;
    h = height;
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
    std::cout << " ******** (new w, new h) = " << neww << " " << newh << std::endl;
    if( neww != w || newh != h )
    {
        // resized = wxBitmap( image.Scale( neww, newh /*, wxIMAGE_QUALITY_HIGH*/ ) );
        // resized = wxBitmap( image2.Scale( neww, newh /*, wxIMAGE_QUALITY_HIGH*/ ) );
        resized = wxBitmap( image2.Scale( neww, newh , wxIMAGE_QUALITY_HIGH ) );
        w = neww;
        h = newh;
        dc.DrawBitmap( resized, 0, 0, false );
    }else{
        resized = wxBitmap( image2 );
        // resized = wxBitmap( image2.Scale( neww, newh , wxIMAGE_QUALITY_HIGH) );
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

void MandelbrotPanel::update() {
    std::cout << " MandelbrotPanel::update " << std::endl;
    unsigned char *buffer = _imageBuffer.getBuffer();
    // As a diagnostic, corrupt the buffer
    /*
    for (int i = 0; i < 400*400; i += 3) {
        buffer[i] = 0;
    }
    */
    std::cout << "update : (width, height) = " << _imageBuffer.getWidth() << " " << _imageBuffer.getHeight() << std::endl;
    image2.Create(_imageBuffer.getWidth(), _imageBuffer.getHeight(), buffer);

    // image2.SetData(buffer, _imageBuffer.getWidth(), _imageBuffer.getHeight());
    //_frame->Show(TRUE);
    // image2.Create( _imageBuffer.getWidth(), _imageBuffer.getHeight(), buffer);
    // paintNow();
    Refresh();
    // Update();
    // Layout();
}

void MandelbrotPanel::moveImageBufferHere(ImageBuffer<unsigned char> imageBuffer)
{
    _imageBuffer = std::move(imageBuffer);
    w = _imageBuffer.getWidth();
    h = _imageBuffer.getHeight();
    debug();
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

bool MandelbrotApp::OnInit()
{
    // make sure to call this first
    wxInitAllImageHandlers();
    

    _frame = new MainFrame(wxT("Mandelbrot Viewer"), wxPoint(50,50), wxSize(500,500));
    _frame->Show(TRUE);
    SetTopWindow(_frame);

    // Start the AutoSaver
    // AutoSave *autoSave = new AutoSave();
    // autoSave->runTimerOnThread();
    // autoSave->runMonitorOnThread();
    
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


    // Maximize();  // Maximize the window

}

void MainFrame::NewFile(wxCommandEvent& WXUNUSED(event))
{
    // Clear the panel display
    // MainEditBox->Clear();
    // reset the path of our current open file
    _currentDocPath = ::wxGetCwd();
    // Set the Title to reflect the file open
    _currentFileName = "mandelbrot.ppm";
    SetTitle(_currentFileName);

 
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
        _currentDocPath = OpenDialog->GetPath();

        // Sets our current document to the file the user selected
        // MainEditBox->LoadFile(CurrentDocPath); //Opens that file
        // Set the Title to reflect the  file open
        SetTitle(wxString("Edit - ") << OpenDialog->GetFilename());
            FileType fileType = PPM;
            _currentFileName = OpenDialog->GetFilename();
            std::cout << " file name " << _currentFileName << std::endl;
            // Need to fix the API here
            // Create an Imagebuffer that is red.
            // Use move semantics to move the buffer into the MandelbrotPanel
            // Once this works 
            ImageBuffer<unsigned char> imageBuffer = readFile(PPM, std::string(_currentFileName.mb_str()));
            
            // Diagnostic
            
            /*
            for (int i = 0; i < imageBuffer.getWidth(); ++i) {
                for (int j = 0; j < imageBuffer.getHeight(); ++j) {
                    imageBuffer.setRed(i,j,(unsigned char)255);
                    imageBuffer.setGreen(i,j,(unsigned char)0);
                    imageBuffer.setBlue(i,j,(unsigned char)0);
                }
            }
            */

            // Use move semantics to copy the buffer into the MandelbrotPanel class
            _mandelbrotPanel->moveImageBufferHere(imageBuffer);
            _mandelbrotPanel->update();

            // Note that at this point tempora
            // ImageBuffer<unsigned char> *tempFoo;
            // ImageIO *imageIO = ImageIO::getImageWriter(fileType, std::string(_currentFileName.mb_str()), tempFoo);
            // ImageBuffer<unsigned char> *tempBuffer = imageIO->read();

            // Render the image buffer to the panel
                
    }

}

void MainFrame::CloseFile(wxCommandEvent& WXUNUSED(event))
{
    // Clear the Text Box
    // MainEditBox->Clear();
    // Reset the current File being edited
    _currentDocPath = ::wxGetCwd();
    // Set the Title to reflect the file open
    std::string fName = "temp_close_file.ppm";
    getMandelbrotPanel()->getMandelbrotPointer()->write(PPM, fName);

    SetTitle("Edit - untitled *");
}

void MainFrame::SaveFile(wxCommandEvent& WXUNUSED(event))
{
    // Save to the already-set path for the document
    // TODO: Save the file
    std::cout << " currentDocPath = " << _currentDocPath << std::endl;
    std::string fName = "temp3.ppm";
    getMandelbrotPanel()->getMandelbrotPointer()->write(PPM, fName);
 
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
        std::string fileName = std::string(SaveDialog->GetFilename());

        getMandelbrotPanel()->getMandelbrotPointer()->write(PPM, fileName);
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
        // std::cout << "Return message from thread is: " << returnMessage << std::endl;
    }

    // Clean up after ourselves
    SaveDialog->Destroy();
}

void MainFrame::Quit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE); // Close the window
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
         std::cout << " error! " << std::endl; 
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
      return imageBuffer;
   } else {
        ImageBuffer<unsigned char> imageBuffer;
        return imageBuffer;
    }

}