#ifndef GDFLEXBOX_H
#define GDFLEXBOX_H

#include <Godot.hpp>
#include <Reference.hpp>
#include <FuncRef.hpp>

#include <wrapper/FNode.hh>
#include <helpers/current_function.h>

namespace godot
{

#define GODOT_LOG(level, message)                                                  \
    switch (level)                                                                 \
    {                                                                              \
    case 0:                                                                        \
        Godot::print(message);                                                     \
        break;                                                                     \
    case 1:                                                                        \
        Godot::print_warning(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__); \
        break;                                                                     \
    case 2:                                                                        \
        Godot::print_error(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);   \
        break;                                                                     \
    }

    class Flexbox : public Reference, public MeasureCallback, public DirtiedCallback
    {
        GODOT_CLASS(Flexbox, Reference)
    public:
        FConfig *m_config;
        FNode *m_node;

    public:
        static void _register_methods();

        void _init();

        Flexbox();
        ~Flexbox();

        void dirtied();
        Size measure(
            float width,
            int widthMode,
            float height,
            int heightMode);

        void insertChild(FNode* child, unsigned index);
    };
}
#endif
