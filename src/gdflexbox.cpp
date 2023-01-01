#include "gdflexbox.h"

using namespace godot;

void Flexbox::_register_methods()
{
    GODOT_LOG(0, "Flexbox::_register_methods");
}
Flexbox::Flexbox()
{
    GODOT_LOG(0, "Flexbox constructor");
}

Flexbox::~Flexbox()
{
    GODOT_LOG(0, "Flexbox destructor");
}

void Flexbox::_init()
{
    m_config = FConfig::create();
    m_node = FNode::createWithConfig(m_config);
    m_node->setAlignItems(YGAlignFlexStart);
    m_node->setWidth(100);
    m_node->setHeight(100);
    GODOT_LOG(0, "Flexbox::_init");
}

void Flexbox::dirtied()
{
    GODOT_LOG(0, "Flexbox::dirtied");
}

// measure callback
Size Flexbox::measure(float width, int widthMode, float height, int heightMode)
{
    GODOT_LOG(0, "Flexbox::measure");
    return Size(0, 0);
}

void Flexbox::insertChild(FNode *child, unsigned index)
{
    GODOT_LOG(0, "Flexbox::insertChild");
    m_node->insertChild(child, index);
}