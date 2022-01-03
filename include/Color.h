#ifndef COLOR_H
#define COLOR_H


class Color
{
    public:
        Color();
        Color(float red, float green, float blue);
        virtual ~Color();

        float Getred() { return m_red; }
        void Setred(float val) { m_red = val; }
        float Getgreen() { return m_green; }
        void Setgreen(float val) { m_green = val; }
        float Getblue() { return m_blue; }
        void Setblue(float val) { m_blue = val; }

    protected:

    private:
        float m_red;
        float m_green;
        float m_blue;
};

#endif // COLOR_H
