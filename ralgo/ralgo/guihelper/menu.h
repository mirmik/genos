#ifndef RALGO_MENUITEM_H
#define RALGO_MENUITEM_H

namespace ralgo
{
    namespace gui
    {
        class paint_visitor
        {
            void paint(widget *wdg);
        };

        class viewcontext
        {
            widget *focused_widget;

        public:
            void next_button_handle();
            void prev_button_handle();
            void click_button_handle();
        };

        class widget
        {
            widget *parent;

        public:
            virtual click(viewcontext *context) = 0;
            virtual prev(viewcontext *context) = 0;
            virtual next(viewcontext *context) = 0;
        };

        class listed_menu : public widget
        {
            int current_item = 0;
            std::vector<std::unique_ptr<menu_item>> items;

        public:
            void add_item(std::string str, std::unique_ptr<widget> widget) {}
        };

        class text_menu_item : public menu_item
        {
            std::string value;
            igris::delegate<const std::string &> text_edited;
        };

        class choise_menu_item : public menu_item
        {
            std::vector<ralgo::gui::widget> variants;
            igris::delegate<int index, ralgo::gui::widget *> index_changed;
        };
    }
}

#endif