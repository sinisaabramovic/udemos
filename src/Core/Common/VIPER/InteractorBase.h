//
//  InteractorBase.h
//  CommonPod
//
//  Created by Sinisa Abramovic on 15.03.2023..
//

#ifndef InteractorBase_h
#define InteractorBase_h

#include "ViperIncludes.h"
#include "EntityBase.h"

using namespace std;

template <typename T>
class InteractorBase {
public:
    InteractorBase(shared_ptr<EntityBase<T>> entity) : m_entity(entity) {}
    std::shared_ptr<const T> getData() const { return m_entity->getData(); }
    void setData(const T& data) { m_entity->setData(data); }
private:
    shared_ptr<EntityBase<T>> m_entity;
};

#endif /* InteractorBase_h */
