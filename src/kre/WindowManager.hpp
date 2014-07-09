/*
	Copyright (C) 2003-2013 by Kristina Simpson <sweet.kristas@gmail.com>
	
	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	   1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgement in the product documentation would be
	   appreciated but is not required.

	   2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	   3. This notice may not be removed or altered from any source
	   distribution.
*/

#pragma once

#include <string>

#include "../Color.hpp"
#include "DisplayDevice.hpp"
#include "Material.hpp"
#include "Renderable.hpp"
#include "WindowManagerFwd.hpp"

namespace KRE
{
	class WindowManager
	{
	public:
		enum FullScreenMode {
			WINDOWED_MODE,
			FULLSCREEN_WINDOWED_MODE,
			FULLSCREEN_MODE,
		};
		explicit WindowManager(const std::string& title);
		virtual ~WindowManager();
		void createWindow(unsigned width, unsigned height);
		void destroyWindow();
		
		virtual bool setWindowSize(unsigned width, unsigned height) = 0;
		virtual bool autoWindowSize(unsigned& width, unsigned& height) = 0;
		virtual bool setLogicalWindowSize(unsigned width, unsigned height) = 0;

		virtual void setWindowTitle(const std::string& title) = 0;
		virtual void setWindowIcon(const std::string& name) = 0;

		virtual unsigned getWindowID() const = 0;

		virtual void render(const Renderable* r) const = 0;

		virtual void swap() = 0;

		void mapMousePosition(int* x, int* y);

		void enable16bpp(bool bpp=true);
		void enableMultisampling(bool multi_sampling=true, unsigned samples=4);
		void enableResizeableWindow(bool en=true);
		void setFullscreenMode(FullScreenMode mode);
		void enableVsync(bool en=true);

		bool use16bpp() const { return use_16bpp_; }
		bool useMultiSampling() const { return use_multi_sampling_; }
		unsigned multiSamples() const { return samples_; }
		bool resizeable() const { return is_resizeable_; }
		FullScreenMode fullscreenMode() const { return fullscreen_mode_; }
		bool vSync() const { return use_vsync_; }

		unsigned width() const { return width_; }
		unsigned height() const { return height_; }

		unsigned logicalWidth() const { return logical_width_; }
		unsigned logicalHeight() const { return logical_height_; }

		const std::string& getTitle() const { return title_; }

		void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255);
		void setClearColor(float r, float g, float b, float a=1.0f);
		void setClearColor(const Color& color);
		
		Color getClearColor() const { return clear_color_; }

		virtual void clear(ClearFlags f) = 0;

		virtual void setViewPort(int x, int y, unsigned width, unsigned height) = 0;

		static WindowManagerPtr factory(const std::string& title, const std::string& wnd_hint="", const std::string& rend_hint="");
		static std::vector<WindowManagerPtr> getWindowList();
		static WindowManagerPtr getWindowFromID(unsigned id);
		static WindowManagerPtr getMainWindow();
	protected:
		unsigned width_;
		unsigned height_;
		unsigned logical_width_;
		unsigned logical_height_;
		Color clear_color_;

		DisplayDevicePtr display_;
	private:
		virtual void changeFullscreenMode() = 0;
		virtual void handleSetClearColor() = 0;
		virtual bool handleLogicalWindowSizeChange() = 0;
		virtual void doCreateWindow(unsigned width, unsigned height) = 0;
		virtual void doDestroyWindow() = 0;

		bool use_16bpp_;
		bool use_multi_sampling_;
		unsigned samples_;
		bool is_resizeable_;
		FullScreenMode fullscreen_mode_;
		std::string title_;
		bool use_vsync_;

		WindowManager();
		WindowManager(const WindowManager&);
	};
}
