#include "cosmos/ecology.hpp"
#include <cstdlib>
#include <iostream>
static_assert(sizeof(cosmos::world::Ecosystem)<=512U,"ecosystem patch must stay within 512 bytes");
namespace {
int failures=0;
void check(bool ok,const char* message) { if(!ok) { std::cerr<<"FAIL: "<<message<<'\n'; ++failures; } }
bool same(const cosmos::world::Ecosystem& a,const cosmos::world::Ecosystem& b) {
    for(int y=0;y<cosmos::world::Ecosystem::kHeight;++y)
        for(int x=0;x<cosmos::world::Ecosystem::kWidth;++x) {
            const auto* lhs=a.cell(x,y); const auto* rhs=b.cell(x,y);
            if(!lhs||!rhs||lhs->kind!=rhs->kind||lhs->energy!=rhs->energy||lhs->age!=rhs->age) return false;
        }
    return a.tick_count()==b.tick_count()&&a.rain_ticks_remaining()==b.rain_ticks_remaining()&&a.random_state()==b.random_state();
}
void deterministic_initialization() {
    cosmos::world::Ecosystem a,b; a.initialize(0x12345678U); b.initialize(0x12345678U);
    check(a.valid()&&b.valid(),"initialized state is valid");
    check(same(a,b),"same seed creates same initial ecosystem");
    check(a.population().plants>0&&a.population().grazers>0&&a.population().predators>0,"starter ecosystem has three trophic groups");
}
void deterministic_ticks() {
    cosmos::world::Ecosystem a,b; a.initialize(77U); b.initialize(77U); a.make_rain(12); b.make_rain(12);
    for(int i=0;i<160;++i) { a.step(); b.step(); check(a.valid(),"state remains valid after a tick"); check(same(a,b),"same inputs produce same state"); }
}
void planting_bounds() {
    cosmos::world::Ecosystem world; world.initialize(4U); const auto before=world.population().plants;
    world.seed_plant(-1,0); world.seed_plant(cosmos::world::Ecosystem::kWidth,0); world.seed_plant(0,cosmos::world::Ecosystem::kHeight);
    check(world.population().plants==before,"out-of-bounds planting does nothing");
    bool planted=false;
    for(int y=0;y<cosmos::world::Ecosystem::kHeight&&!planted;++y)
        for(int x=0;x<cosmos::world::Ecosystem::kWidth&&!planted;++x)
            if(world.cell(x,y)->kind==cosmos::world::CellKind::Grass) planted=world.seed_plant(x,y);
    check(planted,"planting on a grass cell succeeds");
}
void rain_lifecycle() {
    cosmos::world::Ecosystem world; world.initialize(5U); world.make_rain(3);
    check(world.rain_ticks_remaining()==3,"rain begins at requested duration");
    world.step(); check(world.rain_ticks_remaining()==2,"rain decrements each tick");
    world.step(); world.step(); check(world.rain_ticks_remaining()==0,"rain stops at zero");
}
void fixed_population_capacity() {
    cosmos::world::Ecosystem world; world.initialize(0xBADC0DEU);
    for(int i=0;i<1000;++i) world.step();
    const auto p=world.population();
    check(static_cast<unsigned>(p.grass)+p.water+p.plants+p.grazers+p.predators==cosmos::world::Ecosystem::kCellCount,
          "population counts account for every region cell");
}
}
int main() {
    deterministic_initialization(); deterministic_ticks(); planting_bounds(); rain_lifecycle(); fixed_population_capacity();
    if(failures) { std::cerr<<failures<<" ecology test(s) failed\n"; return EXIT_FAILURE; }
    std::cout<<"COSMOS ecology tests: all passed\n"; return EXIT_SUCCESS;
}
