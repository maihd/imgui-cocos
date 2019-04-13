#pragma once

#include <cocos2d.h>
#include "imgui/imgui.h"

namespace cocos2d
{
    class ImGuiGLView : public GLView
    {
    public:
        static ImGuiGLView* create(const std::string& viewName);
        static ImGuiGLView* create(const std::string& viewName, bool resizable);
        static ImGuiGLView* createWithRect(const std::string& viewName, Rect size, float frameZoomFactor = 1.0f, bool resizable = false);
        static ImGuiGLView* createWithFullScreen(const std::string& viewName);
        static ImGuiGLView* createWithFullScreen(const std::string& viewName, const GLFWvidmode &videoMode, GLFWmonitor *monitor);

        /*
        *frameZoomFactor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
        */

        //void resize(int width, int height);

        float getFrameZoomFactor() const override;
        //void centerWindow();

        virtual void setViewPortInPoints(float x , float y , float w , float h) override;
        virtual void setScissorInPoints(float x , float y , float w , float h) override;
        virtual Rect getScissorRect() const override;

        bool windowShouldClose() override;
        void pollEvents() override;
        GLFWwindow* getWindow() const { return _mainWindow; }

        bool isFullscreen() const;
        void setFullscreen();
        void setFullscreen(int monitorIndex);
        void setFullscreen(const GLFWvidmode &videoMode, GLFWmonitor *monitor);
        void setWindowed(int width, int height);
        int getMonitorCount() const;
        Size getMonitorSize() const;

        /* override functions */
        virtual bool isOpenGLReady() override;
        virtual void end() override;
        virtual void swapBuffers() override;
        virtual void setFrameSize(float width, float height) override;
        virtual void setIMEKeyboardState(bool bOpen) override;

        /*
        * Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
        */
        void setFrameZoomFactor(float zoomFactor) override;
        /**
        * Hide or Show the mouse cursor if there is one.
        */
        virtual void setCursorVisible(bool isVisible) override;
        /** Retina support is disabled by default
        *  @note This method is only available on Mac.
        */
        void enableRetina(bool enabled);
        /** Check whether retina display is enabled. */
        bool isRetinaEnabled() const { return _isRetinaEnabled; };
        
        /** Get retina factor */
        int getRetinaFactor() const override { return _retinaFactor; }
        
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        HWND getWin32Window() { return glfwGetWin32Window(_mainWindow); }
    #endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */
        
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        id getCocoaWindow() override { return glfwGetCocoaWindow(_mainWindow); }
    #endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

    protected:
        ImGuiGLView(bool initglfw = true);
        virtual ~ImGuiGLView();

        bool initWithRect(const std::string& viewName, Rect rect, float frameZoomFactor, bool resizable);
        bool initWithFullScreen(const std::string& viewName);
        bool initWithFullscreen(const std::string& viewname, const GLFWvidmode &videoMode, GLFWmonitor *monitor);

        bool initGlew();

        void updateFrameSize();

        // GLFW callbacks
        void onGLFWError(int errorID, const char* errorDesc);
        void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify);
        void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y);
        void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y);
        void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void onGLFWCharCallback(GLFWwindow* window, unsigned int character);
        void onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y);
        void onGLFWframebuffersize(GLFWwindow* window, int w, int h);
        void onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height);
        void onGLFWWindowIconifyCallback(GLFWwindow* window, int iconified);
        void onGLFWWindowFocusCallback(GLFWwindow* window, int focused);

        bool _captured;
        bool _supportTouch;
        bool _isInRetinaMonitor;
        bool _isRetinaEnabled;
        int  _retinaFactor;  // Should be 1 or 2

        float _frameZoomFactor;

        GLFWwindow* _mainWindow;
        GLFWmonitor* _monitor;

        std::string _glfwError;

        float _mouseX;
        float _mouseY;

        friend class ImGuiEventHandler;
        
    public:
        // View will trigger an event when window is resized, gains or loses focus
        static const std::string EVENT_WINDOW_RESIZED;
        static const std::string EVENT_WINDOW_FOCUSED;
        static const std::string EVENT_WINDOW_UNFOCUSED;

    private:
        CC_DISALLOW_COPY_AND_ASSIGN(ImGuiGLView);
    };

    class ImGuiLayer : public Layer
    {
    public:
        CREATE_FUNC(ImGuiLayer);

    public:
        std::function<void(void)> onGui;

    protected:
        void onDrawImGui(void);
        void visit(Renderer* renderer, const Mat4& trasnform, uint32_t flags) override;

    protected:
        CustomCommand _drawImGuiCommand;
    };
};