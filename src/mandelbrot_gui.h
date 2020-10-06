#ifndef MANDELBROT_GUI_H_
#define MANDELBROT_GUI_H_

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/sizer.h>

#include "AutoSave.h"
#include "ImageBuffer.h"
#include <string>
#include <thread>

class MainFrame;
class Mandelbrot;


class MandelbrotPanel : public wxPanel
{
private:
    std::unique_ptr<Mandelbrot> _mandelbrotPointer;
    AutoSave *_autoSave;
    // Mandelbrot *_mandelbrotPointer;
    wxImage image;
    wxImage image2;
    wxBitmap resized;
    ImageBuffer<unsigned char> _imageBuffer;
    int w, h;
    
public:
    MandelbrotPanel(wxFrame* parent, wxString file, wxBitmapType format);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);
    void update();

    Mandelbrot *getMandelbrotPointer() { return _mandelbrotPointer.get(); }
    void moveImageBufferHere(ImageBuffer<unsigned char> imageBuffer);
    void debug(); // This should be removed eventually
    ImageBuffer<unsigned char> getImageBuffer();

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

    // This should be moved into a separate class.
    ImageBuffer<unsigned char> readFile(FileType type, std::string fileName);
    // This should be moved into a separate class
    void writeFile(FileType type, std::string fileName, ImageBuffer<unsigned char> imageBuffer);

    DECLARE_EVENT_TABLE()
private:
// The Path to the file we have open
    wxString _currentDocPath;
    wxString _currentFileName;

    wxMenuBar *_mainMenu;
    MandelbrotPanel *_mandelbrotPanel;  // should convert to shared pointer
    AutoSave *_autoSave;
    std::thread _saveThread;
    std::shared_ptr<SaveJob> _saveJob;
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
