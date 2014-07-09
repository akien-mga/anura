/*
	Copyright (C) 2003-2014 by David White <davewx7@gmail.com>
	
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

#include <boost/intrusive_ptr.hpp>

#include "kre/SceneObject.hpp"

#include "formula_callable.hpp"
#include "formula_callable_definition.hpp"

namespace graphics
{
	class DrawPrimitive : public game_logic::FormulaCallable, public KRE::SceneObject
	{
	public:
		static boost::intrusive_ptr<DrawPrimitive> create(const variant& v);
		explicit DrawPrimitive(const variant& v);
	private:
		DECLARE_CALLABLE(DrawPrimitive);
		virtual KRE::DisplayDeviceDef doAttach(const KRE::DisplayDevicePtr& dd) override;
		std::string shader_name_;
	};

	typedef boost::intrusive_ptr<DrawPrimitive> DrawPrimitivePtr;
	typedef boost::intrusive_ptr<const DrawPrimitive> ConstDrawPrimitivePtr;
}
