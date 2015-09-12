/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2015 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_TOOLTIP_HPP
#define TGUI_TOOLTIP_HPP


#include <TGUI/Widgets/Label.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    class ToolTipRenderer;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API ToolTip : public Label
    {
    public:

        typedef std::shared_ptr<ToolTip> Ptr; ///< Shared widget pointer
        typedef std::shared_ptr<const ToolTip> ConstPtr; ///< Shared constant widget pointer


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ToolTip();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another tooltip
        ///
        /// @param tooltip  The other tooltip
        ///
        /// @return The new tooltip
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static ToolTip::Ptr copy(ToolTip::ConstPtr tooltip);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the time that the mouse has to stand still before the tooltip becomes visible
        ///
        /// @param timeToDisplay Time before displaying tooltip
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void setTimeToDisplay(const sf::Time& timeToDisplay)
        {
            m_timeToDisplay = timeToDisplay;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the time that the mouse has to stand still before the tooltip becomes visible
        ///
        /// @return Time before displaying tooltip
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static sf::Time getTimeToDisplay()
        {
            return m_timeToDisplay;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() override
        {
            return std::make_shared<ToolTip>(*this);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        static sf::Time m_timeToDisplay;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TOOLTIP_HPP
