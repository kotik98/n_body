#include <iostream>
#include "GLBall.hpp"
#include "NBodyScene.hpp"
#include <vector>
#include <cmath>

using namespace std;

const double G = 1.;

class Ball : public GLBall {
protected:
    double vx;
    double vy;
    double x;
    double y;
    double mass;

public:
    Ball(double x, double y, double vx, double vy, double m)
            : x(x), y(y), vx(vx), vy(vy), mass(m) {}

    void move(double& dt, vector<Ball>& curr, int& index) {
        for (int i = index + 1; i < curr.size(); i++){
            this->interact(dt, curr[i]);
        }
        x += vx * dt;
        y += vy * dt;
        cout << index << ' ' << x << ' ' << y << "             " << sqrt(pow(x, 2) + pow(y, 2)) << endl;
    }

    void interact(double& dt, Ball& curr){
        double r2 = (pow((this->getX() - curr.getX()), 2) + pow((this->getY() - curr.getY()), 2));
        if (r2 < 4){
            return;
        }
        double F = G * this->mass * curr.mass / r2;
        double ax = (curr.getX() - this->getX()) / sqrt(r2) / this->mass * F;
        double ay = (curr.getY() - this->getY()) / sqrt(r2) / this->mass * F;
        vx += ax * dt;
        vy += ay * dt;
        ax = (-curr.getX() + this->getX()) / sqrt(r2) / curr.mass * F;
        ay = (-curr.getY() + this->getY()) / sqrt(r2) / curr.mass * F;
        curr.vx += ax * dt;
        curr.vy += ay * dt;
    }

    double getX() const override {
        return x;
    }
    double getY() const override {
        return y;
    }
    double getR() const override {
        return 1;
    }
};

class Scene : public NBodyScene {
protected:
    std::vector<Ball> bodies;

public:

    unsigned int getNumberOfBodies() const override {
        return bodies.size();
    }

    const GLBall& getBody(unsigned int number) const override {
        return bodies.at(number);
    }

    void doTimeStep() override {
        int i = 0;
        double t = 0.1;
        for(auto& b  : bodies) {
            b.move(t, bodies, i);
            i++;
        }
    }

    void initScene() {
        int n, x, y, vx, vy, m;
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> x >> y >> vx >> vy >> m;
            bodies.push_back(Ball(x, y, vx, vy, m));
        }

        // круг
        //bodies.push_back(Ball(0, 0, 0, 0, 1000));
        //bodies.push_back(Ball(10, 0, 0, 10, 1));

        // качели
        //bodies.push_back(Ball(0, 0, 0, 0, 100));
        //bodies.push_back(Ball(10, 0, 0, 0, 1));
    }
};

NBodyScene* getScene()
{
    Scene* s = new Scene();
    s->initScene();
    return s;
}

int main()
{

    Scene* scene = (Scene*)getScene();

    int i = 0;
    while (i < 1000) {
        scene->doTimeStep();
        cout << i << endl;
        i++;
    }

    delete scene;
    return 0;
};