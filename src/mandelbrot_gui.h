#ifndef MANDELBROT_GUI_H_
#define MANDELBROT_GUI_H_

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/sizer.h>


#include "ImageBuffer.h"

class MainFrame;
class Mandelbrot;


class MandelbrotPanel : public wxPanel
{
    Mandelbrot *mandelbrotPointer;
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
    void update(ImageBuffer<unsigned char> *imageBuffer);
    Mandelbrot *getMandelbrotPointer() { return mandelbrotPointer; }
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
private:
    MainFrame *_frame;
public:
    bool OnInit();
};

class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& position, const wxSize& size);

    void Quit( wxCommandEvent& event );
    void NewFile( wxCommandEvent& event );
    void OpenFile( wxCommandEvent& event );
    void SaveFile( wxCommandEvent& event );
    void SaveFileAs( wxCommandEvent& event );
    void CloseFile( wxCommandEvent& event );

    MandelbrotPanel *getMandelbrotPanel() { return _mandelbrotPanel; }
// The Path to the file we have open
    wxString CurrentDocPath;

    DECLARE_EVENT_TABLE()
private:
    wxMenuBar *_mainMenu;
    MandelbrotPanel *_mandelbrotPanel;
};

enum
{
        // declares an id which will be used to call our button
        TEXT_Main = wxID_HIGHEST + 1,
        MENU_New,
        MENU_Open,
        MENU_Close,
        MENU_Save,
        MENU_SaveAs,
        MENU_Quit
};


#endif // MANDELBROT_GUI_H_
