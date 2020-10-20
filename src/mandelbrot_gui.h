#ifndef MANDELBROT_GUI_H_
#define MANDELBROT_GUI_H_

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/sizer.h>

#include "ImageBuffer.h"
#include "MandelbrotPanel.h"

#include <string>
#include <thread>
#include <future>

class MainFrame;
class Mandelbrot;

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

    void SliderX(wxCommandEvent& event);
    void SliderY(wxCommandEvent& event);
    void Scale(wxCommandEvent& event);

    MandelbrotPanel *getMandelbrotPanel() { return _mandelbrotPanel; }

    // This should be moved into a separate class.
    ImageBuffer<PixelType> readFile(FileType type, std::string fileName);

    DECLARE_EVENT_TABLE()
private:
// The Path to the file we have open
    wxString _currentDocPath;
    wxString _currentFileName;

    wxMenuBar *_mainMenu;
    MandelbrotPanel *_mandelbrotPanel;  // should convert to shared pointer

    std::thread _saveThread;
    std::shared_ptr<SaveJob> _saveJob;
    std::vector<std::shared_ptr<SaveJob>> _saveJobs;

    void writeFile(std::promise<std::string> &&promise, std::shared_ptr<SaveJob> saveJob);

    wxSlider *_sliderX;
    wxSlider *_sliderY;
    wxSlider *_scale;
    
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
        MENU_Quit,
        SLIDER_X,
        SLIDER_Y,
        SCALE
};


#endif // MANDELBROT_GUI_H_
