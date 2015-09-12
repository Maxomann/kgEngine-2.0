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


#ifndef TGUI_PANEL_HPP
#define TGUI_PANEL_HPP


#include <TGUI/Container.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Panel widget
    ///
    /// The panel is a static container of widgets. The background color can be solid or transparent.
    /// It can be used for having radio button groups or any other reason to group certain widgets.
    ///
    /// Signals:
    ///     - MousePressed (the left mouse button was pressed on top of the panel)
    ///         * Optional parameter sf::Vector2f: Mouse position relative to the panel position
    ///         * Uses Callback member 'mouse'
    ///
    ///     - MouseReleased (the left mouse button was released on top of the panel)
    ///         * Optional parameter sf::Vector2f: Mouse position relative to the panel position
    ///         * Uses Callback member 'mouse'
    ///
    ///     - Clicked (you left clicked the panel)
    ///         * Optional parameter sf::Vector2f: Mouse position relative to the panel position
    ///         * Uses Callback member 'mouse'
    ///
    ///     - Inherited signals from Container
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Panel : public Container
    {
    public:

        typedef std::shared_ptr<Panel> Ptr; ///< Shared widget pointer
        typedef std::shared_ptr<const Panel> ConstPtr; ///< Shared constant widget pointer


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        ///
        /// @param size  Size of the panel
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Panel(const Layout2d& size = {100, 100});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor
        ///
        /// @param width   Width of the widget
        /// @param height  Height of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Panel(const Layout& width, const Layout& height);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another panel
        ///
        /// @param panel  The other panel
        ///
        /// @return The new panel
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static Panel::Ptr copy(Panel::ConstPtr panel);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the background color of the panel.
        ///
        /// @param backgroundColor  New background color
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setBackgroundColor(const sf::Color& backgroundColor)
        {
            m_backgroundColor = backgroundColor;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the background color of the panel.
        ///
        /// @return The current background color
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getBackgroundColor() const
        {
            return m_backgroundColor;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the transparency of the widget.
        ///
        /// @param transparency  The transparency of the widget.
        ///                      0 is completely transparent, while 255 (default) means fully opaque.
        ///
        /// Note that this will only change the transparency of the images. The parts of the widgets that use a color will not
        /// be changed. You must change them yourself by setting the alpha channel of the color.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setTransparency(unsigned char transparency) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool mouseOnWidget(float x, float y) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMousePressed(float x, float y) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMouseReleased(float x, float y) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() override
        {
            return std::make_shared<Panel>(*this);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draws the widget on the render target.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        sf::Color    m_backgroundColor = {220, 220, 220};

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        friend class Container;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_PANEL_HPP
