//
//  ViewBase.h
//  CommonPod
//
//  Created by Sinisa Abramovic on 15.03.2023..
//

#ifndef ViewBase_h
#define ViewBase_h

#include "PresenterBase.h"

using namespace std;

template <typename T>
class ViewBase {
public:
    ViewBase(shared_ptr<PresenterBase<T>> presenter) : m_presenter(presenter) {
        m_presenter->attachView(bind(&ViewBase::onDataChanged, this, placeholders::_1));
    }

    void onDataChanged(const T& data) {
        cout << "View updated: " << data << endl;
    }

private:
    shared_ptr<PresenterBase<T>> m_presenter;
};

#endif /* ViewBase_h */
