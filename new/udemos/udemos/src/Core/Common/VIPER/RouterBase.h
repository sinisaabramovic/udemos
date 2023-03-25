//
//  RouterBase.h
//  CommonPod
//
//  Created by Sinisa Abramovic on 15.03.2023..
//

#ifndef RouterBase_h
#define RouterBase_h

#include "ViewBase.h"
#include "EntityBase.h"
#include "InteractorBase.h"
#include "PresenterBase.h"

template <typename T>
class RouterBase {
public:
    RouterBase(const string& routePath) : m_routePath(routePath) {
        m_entity = make_shared<EntityBase<T>>();
        m_interactor = make_shared<InteractorBase<T>>(m_entity);
        m_presenter = make_shared<PresenterBase<T>>(m_interactor);
        m_view = make_shared<ViewBase<T>>(m_presenter);
    }
    
    void updateData(const T& data) {
        m_presenter->updateData(data);
    }
    
    const string& getRoutePath() const { return m_routePath; }
    
private:
    shared_ptr<EntityBase<T>> m_entity;
    shared_ptr<InteractorBase<T>> m_interactor;
    shared_ptr<PresenterBase<T>> m_presenter;
    shared_ptr<ViewBase<T>> m_view;
    string m_routePath;
};

#endif /* RouterBase_h */
