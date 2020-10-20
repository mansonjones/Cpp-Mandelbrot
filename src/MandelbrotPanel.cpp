
#include "AutoSave.h"
#include "Constants.h"
#include "MandelbrotPanel.h"


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

    _autoSave = new AutoSave(this);
   // _autoSave = std::make_unique<AutoSave>(this);
    _autoSave->emitAutoSaveMessagesOnThread();
    
    _autoSave->waitForAutoSaveMessagesOnThread();
    
    _imageBuffer = ImageBuffer<unsigned char>(width, height);
    _mandelbrotPointer->render(_imageBuffer);   

    updateDisplay();
    w = width;
    h = height;
}

MandelbrotPanel::~MandelbrotPanel()
{ 
    delete _autoSave;
}

void MandelbrotPanel::updateDisplay()
{
    size_t bufferSize = _imageBuffer.getRawBufferSize();
    unsigned char *buffer = new unsigned char[bufferSize];
    buffer = _imageBuffer.getRawBuffer();
    // 
    bool imageCreationSuccess = image.Create( _imageBuffer.getWidth(), _imageBuffer.getHeight(), buffer);
    if (!imageCreationSuccess) {
        std::cerr << " image creation did not succeed " << std::endl;
    }
 
    Refresh();
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

    updateDisplay();
}

void MandelbrotPanel::debug()
{
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

    updateDisplay();
}