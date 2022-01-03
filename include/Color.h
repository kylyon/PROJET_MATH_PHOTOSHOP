#ifndef COLOR_H
#define COLOR_H


class Color
{
    public:
        Color();
        Color(float red, float green, float blue, float alpha);
        Color(float red, float green, float blue);
        virtual ~Color();

        float Getred() { return m_red; }
        void Setred(float val) { m_red = val; }
        float Getgreen() { return m_green; }
        void Setgreen(float val) { m_green = val; }
        float Getblue() { return m_blue; }
        void Setblue(float val) { m_blue = val; }
        float Getalpha() { return m_alpha; }
        void Setalpha(float val) { m_alpha = val; }

    protected:

    private:
        float m_red;
        float m_green;
        float m_blue;
        float m_alpha;
};

#endif // COLOR_H
