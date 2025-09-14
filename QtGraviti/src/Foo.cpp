//Example class to proove that GTest works across projects
class FOO {
public:
    FOO() {
        hidden = 0;
    }

    void add(int i)
    {
        hidden += i;
    }

    void subtract(int i)
    {
        hidden -= 1;
    }

    int getVal() {
        return hidden;
    }
private:
    int hidden;
};