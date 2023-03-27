//
//  EntityBase.h
//  CommonPod
//
//  Created by Sinisa Abramovic on 15.03.2023..
//

#ifndef EntityBase_h
#define EntityBase_h

#include "ViperIncludes.h"

using namespace std;

template <typename T>
class EntityBase {
public:
    EntityBase() {}
    void setData(const T& data) {
        m_data.reset(new T(data));
    }
    std::shared_ptr<const T> getData() const { return m_data; }

private:
    std::shared_ptr<const T> m_data;
};

#endif /* Entity_h */
