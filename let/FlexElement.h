#pragma once
#include "Parent.h"
#include "FlexData.h"
class FlexElement :
    public Parent
{
    std::map<Element*, FlexData> _data_map;
    bool _vertical;

public:
    static const bool VERTICAL = true;
    static const bool HORIZONTAL = false;

    FlexElement(UiState* state, bool vertical);

    void _add(std::shared_ptr<Element> element, FlexData data);
    
    void on_removing(Element* element) override;
    void on_bounds_updated() override;
    bool vertical() const;

};

