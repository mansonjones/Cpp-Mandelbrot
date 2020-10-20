#ifndef MANDELBROT_PANEL_H_
#define MANDELBROT_PANEL_H_

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/sizer.h>

#include "AutoSave.h"
#include "ImageBuffer.h"

class AutoSave;
class Mandelbrot;

class MandelbrotPanel : public wxPanel
{
private:
    void updateDisplay();
    std::unique_ptr<Mandelbrot> _mandelbrotPointer;
    AutoSave *_autoSave;
    // std::unique_ptr<AutoSave> _autoSave; 
    wxImage image;
    wxBitmap resized;
    ImageBuffer<PixelType> _imageBuffer;
    int w, h;
    
public:
    MandelbrotPanel(wxFrame* parent, wxString file, wxBitmapType format);
    ~MandelbrotPanel(); 
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    Mandelbrot *getMandelbrotPointer() { return _mandelbrotPointer.get(); }
    void moveImageBufferHere(ImageBuffer<PixelType> imageBuffer);
    ImageBuffer<PixelType> getImageBuffer();

    void recomputeMandelbrot();
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    
    DECLARE_EVENT_TABLE()
};


#endif // MANDELBROT_PANEL_H_
