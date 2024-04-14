#pragma once

class Buyable
{
public:
    virtual const int GetPrice() const { return m_Price; }
private:
    int m_Price = 0;
};