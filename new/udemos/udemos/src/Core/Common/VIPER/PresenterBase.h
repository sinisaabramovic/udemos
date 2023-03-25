//
//  PresenterBase.h
//  CommonPod
//
//  Created by Sinisa Abramovic on 15.03.2023..
//

#ifndef PresenterBase_h
#define PresenterBase_h

#include "InteractorBase.h"

using namespace std;

template <typename T>
class PresenterBase {
public:
    PresenterBase(shared_ptr<InteractorBase<T>> interactor) : m_interactor(interactor) {}

    void attachView(function<void(const T&)> viewUpdateCallback) {
        m_viewUpdateCallback = viewUpdateCallback;
    }

    void updateData(const T& data) {
        m_interactor->setData(data);
        if (m_viewUpdateCallback) {
            m_viewUpdateCallback(*(m_interactor->getData()));
        }
    }

private:
    shared_ptr<InteractorBase<T>> m_interactor;
    function<void(const T&)> m_viewUpdateCallback;
};

#endif /* PresenterBase_h */
