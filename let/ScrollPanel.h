#pragma once
#include "Flexer.h"
#include "FlexData.h"
#include <type_traits>
#include "Parent.h"
#include "HorBar.h"
#include "VertBar.h"
#include "ScrollContent.h"

class ScrollPanel : public Parent, public Observer<const ScrollContent*>
{
    std::shared_ptr<HorBar> _hor;
    std::shared_ptr<VertBar> _ver;
    std::shared_ptr<Element> _element;
    std::shared_ptr<ScrollContent> _content;
    std::shared_ptr<FlexElement> _flexer;
    std::shared_ptr<HorBar> _hor_bar;
public:
    template<
        typename T,
        typename = std::enable_if_t<std::is_base_of_v<Element, T>>,
        typename = std::enable_if_t<std::is_base_of_v<ScrollContent, T>>
    >
    ScrollPanel(std::shared_ptr<T> child, UiState* state) :
        Parent(state)
    {
        _content = child;
        _element = child;
        add(_flexer = Flexer(state, Flexer::VERTICAL)
            .add(child, FlexData::weighted(1))
            .add(_hor_bar = make<HorBar>(
                0.2, 0.1
            ), FlexData::fixed(12))
            .build());
    }

    void update(const ScrollContent* t) override;
    void render(SDL_Renderer* renderer) override;
    void on_bounds_updated() override;
    void on_parent_set() override;

};

