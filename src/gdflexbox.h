#ifndef GDFLEXBOX_H
#define GDFLEXBOX_H

#include <memory>

#include <yoga/Yoga.h>
#include <Godot.hpp>
#include <Reference.hpp>
#include <FuncRef.hpp>
#include <Directory.hpp>

#include <helpers/current_function.h>

namespace godot
{
    class MeasureCallback
    {
    public:
        virtual ~MeasureCallback() {}
        virtual Variant measure(
            float width,
            int widthMode,
            float height,
            int heightMode) = 0;
    };

    class DirtiedCallback
    {
    public:
        virtual ~DirtiedCallback() {}
        virtual void dirtied() = 0;
    };

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
        static void destroy(Flexbox *node);
        static Flexbox *fromYGNode(YGNodeRef nodeRef);

        void _init();
        ~Flexbox();

    public:
        void copy_style(Flexbox *other);
        void set_position_type(int positionType);
        void set_position(int edge, double position);
        void set_position_percent(int edge, double position);

        void set_align_content(int alignContent);
        void set_align_items(int alignItems);
        void set_align_self(int alignSelf);
        void set_flex_direction(int flexDirection);
        void set_flex_wrap(int flexWrap);
        void set_justify_content(int justifyContent);

        void set_margin(int edge, double margin);
        void set_margin_percent(int edge, double margin);
        void set_margin_auto(int edge);

        void set_overflow(int overflow);
        void set_display(int display);

        void set_flex(double flex);
        void set_flex_basis(double flexBasis);
        void set_flex_basis_percent(double flexBasis);
        void set_flex_basis_auto();
        void set_flex_grow(double flexGrow);
        void set_flex_shrink(double flexShrink);

        void set_width(double width);
        void set_width_percent(double width);
        void set_width_auto();
        void set_height(double height);
        void set_height_percent(double height);
        void set_height_auto();

        void set_min_width(double minWidth);
        void set_min_width_percent(double minWidth);
        void set_min_height(double minHeight);
        void set_min_height_percent(double minHeight);

        void set_max_width(double maxWidth);
        void set_max_width_percent(double maxWidth);
        void set_max_height(double maxHeight);
        void set_max_height_percent(double maxHeight);

        void set_aspect_ratio(double aspectRatio);
        void set_border(int edge, double border);
        void set_padding(int edge, double padding);
        void set_padding_percent(int edge, double padding);
        void set_gap(int gutter, double gapLength);

    public: // Style getters
        int get_position_type(void) const;
        Variant get_position(int edge) const;

        int get_align_content(void) const;
        int get_align_items(void) const;
        int get_align_self(void) const;
        int get_flex_direction(void) const;
        int get_flex_wrap(void) const;
        int get_justify_content(void) const;

        Variant get_margin(int edge) const;

        int get_overflow(void) const;
        int get_display(void) const;

        Variant get_flex_basis(void) const;
        double get_flex_grow(void) const;
        double get_flex_shrink(void) const;

        Variant get_width(void) const;
        Variant get_height(void) const;
        Variant get_min_width(void) const;
        Variant get_min_height(void) const;
        Variant get_max_width(void) const;
        Variant get_max_height(void) const;

        double get_aspect_ratio(void) const;
        double get_border(int edge) const;
        Variant get_padding(int edge) const;
        float get_gap(int gutter);

    public: // Tree hierarchy mutators
        void insert_child(Flexbox *child, unsigned index);
        void remove_child(Flexbox *child);

    public: // Tree hierarchy inspectors
        unsigned get_child_count(void) const;
        Flexbox *get_parent(void);
        Flexbox *get_child(unsigned index);

    public: // Layout inspectors
        double get_computed_left(void) const;
        double get_computed_right(void) const;

        double get_computed_top(void) const;
        double get_computed_bottom(void) const;

        double get_computed_width(void) const;
        double get_computed_height(void) const;

        Variant get_computed_layout(void) const;

        double get_computed_margin(int edge) const;
        double get_computed_border(int edge) const;
        double get_computed_padding(int edge) const;

    public: // Measure func mutators
        void set_measure_func(Ref<FuncRef> funcRef);
        void unset_measure_func(void);

    public: // Measure func inspectors
        // YGSize measure(YGNodeRef nodeRef, float width, int widthMode, float height, int heightMode);
        Dictionary call_measure_func(
            double width,
            int widthMode,
            double height,
            int heightMode);

    public: // Dirtied func mutators
        void set_dirtied_func(Ref<FuncRef> funcRef);
        void unset_dirtied_func(void);

    public: // Dirtied func inspectors
        void call_dirtied_func(void);

    public: // Dirtiness accessors
        void dirtied();
        void mark_dirty(void);
        bool is_dirty(void) const;

    public: // Layout mutators
        void calculate_layout(double width, double height, int direction);
        // void calculate_layout(void);
    public:
        void set_is_reference_baseline(bool isReferenceBaseline);
        bool is_reference_baseline();
        YGNodeRef m_node;
        Ref<FuncRef> m_measureFunc;
        Ref<FuncRef> m_dirtiedFunc;
    };
}
#endif
