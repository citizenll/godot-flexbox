#ifndef GDFLEXBOX_H
#define GDFLEXBOX_H

#include <Godot.hpp>
#include <Reference.hpp>
#include <FuncRef.hpp>

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

    class Flexbox : public Reference
    {
        GODOT_CLASS(Flexbox, Reference)

    public:
        static void _register_methods();

        void _init();
        Flexbox();
        ~Flexbox();
    };

}
#endif
