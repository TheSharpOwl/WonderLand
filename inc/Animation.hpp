#ifndef ANIMATION_H
#define ANIMATION_H
// With help of:
// https://github.com/planetchili/sfml_sprite/blob/master/SFMLspriteAnim/Source.cpp

namespace wonderland {
    class Animation 
    {
        public:
        void update(float dt) {
            time += dt;
            while(time > holdTime) {
                time -= holdTime;
                advance();
            }
        }
        private:
        void advance() {}
        float time;
    };
}
#endif //  ANIMATION_HPP