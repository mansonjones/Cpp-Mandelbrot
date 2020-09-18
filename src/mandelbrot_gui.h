#ifndef MANDELBROT_GUI_H_
#define MANDELBROT_GUI_H_

#include <wx/wx.h>
#include <wx/sizer.h>

class MainFrame;

class MandelbrotPanel : public wxPanel
{
    wxImage image;
    wxImage image2;
    wxBitmap resized;
    int w, h;
    
public:
    MandelbrotPanel(wxFrame* parent, wxString file, wxBitmapType format);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);
    
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

class MandelbrotApp: public wxApp
{
    
    MainFrame *frame;
    MandelbrotPanel * mandelbrotPanel;
public:
    bool OnInit();
};

class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& position, const wxSize& size);
};
#endif // MANDELBROT_GUI_H_
