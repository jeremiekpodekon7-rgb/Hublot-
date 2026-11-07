#include "cosmos/ecology.hpp"
namespace cosmos::world {
bool Ecosystem::inside(int x,int y) noexcept { return x>=0 && x<kWidth && y>=0 && y<kHeight; }
int Ecosystem::index_of(int x,int y) noexcept { return y*kWidth+x; }
std::uint32_t Ecosystem::next_random() noexcept {
    std::uint32_t value=rng_;
    if(value==0U) value=0xA341316CU;
    value^=value<<13U; value^=value>>17U; value^=value<<5U;
    rng_=value; return value;
}
unsigned Ecosystem::random_below(unsigned limit) noexcept {
    if(limit==0U) return 0U;
    const std::uint32_t value=next_random();
    std::uint32_t remainder=0U;
    // Binary remainder avoids a target-runtime division helper on ARM7TDMI.
    for(int bit=31;bit>=0;--bit) {
        remainder=(remainder<<1U)|((value>>static_cast<unsigned>(bit))&1U);
        if(remainder>=limit) remainder-=limit;
    }
    return static_cast<unsigned>(remainder);
}
bool Ecosystem::find_neighbour(int x,int y,unsigned start,int& nx,int& ny) const noexcept {
    constexpr int dx[4]={0,1,0,-1}, dy[4]={-1,0,1,0};
    for(unsigned offset=0;offset<4U;++offset) {
        const unsigned direction=(start+offset)&3U;
        const int cx=x+dx[direction], cy=y+dy[direction];
        if(inside(cx,cy)) { nx=cx; ny=cy; return true; }
    }
    return false;
}
bool Ecosystem::place_random(CellKind kind,std::uint8_t energy) noexcept {
    for(int attempt=0;attempt<kCellCount*2;++attempt) {
        const unsigned position=random_below(kCellCount);
        if(cells_[position].kind==CellKind::Grass) {
            cells_[position]=EcosystemCell{kind,energy,0}; return true;
        }
    }
    for(auto& value:cells_) if(value.kind==CellKind::Grass) {
        value=EcosystemCell{kind,energy,0}; return true;
    }
    return false;
}
void Ecosystem::initialize(std::uint32_t seed) noexcept {
    rng_=seed==0U?0xC001D00Du:seed; ticks_=0U; rain_ticks_=0U;
    cells_.fill(EcosystemCell{CellKind::Grass,0,0});
    for(int i=0;i<18;++i) place_random(CellKind::Plant,3);
    for(int i=0;i<4;++i) place_random(CellKind::Grazer,5);
    for(int i=0;i<2;++i) place_random(CellKind::Predator,6);
}
bool Ecosystem::seed_plant(int x,int y) noexcept {
    if(!inside(x,y)) return false;
    auto& target=cells_[static_cast<unsigned>(index_of(x,y))];
    if(target.kind!=CellKind::Grass) return false;
    target=EcosystemCell{CellKind::Plant,3,0}; return true;
}
void Ecosystem::make_rain(std::uint16_t duration) noexcept { rain_ticks_=duration; }
void Ecosystem::step() noexcept {
    const bool raining=rain_ticks_>0U;
    const CellArray previous=cells_;
    CellArray next=previous; // Snapshot update: newly moved/created entities wait until next tick.
    for(unsigned index=0;index<static_cast<unsigned>(kCellCount);++index) {
        const EcosystemCell& current=previous[index];
        EcosystemCell& result=next[index];
        if(result.kind!=current.kind) continue; // This cell was already consumed/claimed.
        const int x=static_cast<int>(index%static_cast<unsigned>(kWidth));
        const int y=static_cast<int>(index/static_cast<unsigned>(kWidth));
        const unsigned roll=random_below(100U);
        if(current.kind==CellKind::Plant) {
            if(result.age<255U) ++result.age;
            if(result.energy<7U && roll<(raining?38U:14U)) ++result.energy;
            if(result.age>24U && roll<8U) {
                int nx=0,ny=0;
                if(find_neighbour(x,y,random_below(4U),nx,ny)) {
                    const unsigned target=static_cast<unsigned>(index_of(nx,ny));
                    if(previous[target].kind==CellKind::Grass && next[target].kind==CellKind::Grass)
                        next[target]=EcosystemCell{CellKind::Plant,2,0};
                }
            }
        } else if(current.kind==CellKind::Grazer) {
            const std::uint8_t energy=result.energy>0U?static_cast<std::uint8_t>(result.energy-1U):0U;
            result.energy=energy;
            int nx=0,ny=0;
            if(find_neighbour(x,y,random_below(4U),nx,ny)) {
                const unsigned target=static_cast<unsigned>(index_of(nx,ny));
                const CellKind kind=previous[target].kind;
                if(kind==CellKind::Plant && next[target].kind==CellKind::Plant) {
                    next[target]=EcosystemCell{CellKind::Grazer,5,0};
                    result=EcosystemCell{CellKind::Grass,0,0}; continue;
                }
                if(kind==CellKind::Grass && next[target].kind==CellKind::Grass && roll<30U) {
                    next[target]=EcosystemCell{CellKind::Grazer,static_cast<std::uint8_t>(energy>2U?energy:3U),0};
                    result=EcosystemCell{CellKind::Grass,0,0}; continue;
                }
            }
            if(energy==0U || roll<2U) result=EcosystemCell{CellKind::Grass,0,0};
        } else if(current.kind==CellKind::Predator) {
            const std::uint8_t energy=result.energy>0U?static_cast<std::uint8_t>(result.energy-1U):0U;
            result.energy=energy;
            int nx=0,ny=0;
            if(find_neighbour(x,y,random_below(4U),nx,ny)) {
                const unsigned target=static_cast<unsigned>(index_of(nx,ny));
                const CellKind kind=previous[target].kind;
                if(kind==CellKind::Grazer && next[target].kind==CellKind::Grazer) {
                    next[target]=EcosystemCell{CellKind::Predator,6,0};
                    result=EcosystemCell{CellKind::Grass,0,0}; continue;
                }
                if(kind==CellKind::Grass && next[target].kind==CellKind::Grass && roll<12U) {
                    next[target]=EcosystemCell{CellKind::Predator,static_cast<std::uint8_t>(energy>2U?energy:3U),0};
                    result=EcosystemCell{CellKind::Grass,0,0}; continue;
                }
            }
            if(energy==0U || roll<2U) result=EcosystemCell{CellKind::Grass,0,0};
        } else if(current.kind==CellKind::Grass && raining && roll<3U) {
            result=EcosystemCell{CellKind::Plant,1,0};
        }
    }
    cells_=next; ++ticks_; if(rain_ticks_>0U) --rain_ticks_;
}
const EcosystemCell* Ecosystem::cell(int x,int y) const noexcept {
    return inside(x,y)?&cells_[static_cast<unsigned>(index_of(x,y))]:nullptr;
}
PopulationCounts Ecosystem::population() const noexcept {
    PopulationCounts counts{};
    for(const auto& value:cells_) switch(value.kind) {
        case CellKind::Grass: ++counts.grass; break;
        case CellKind::Water: ++counts.water; break;
        case CellKind::Plant: ++counts.plants; break;
        case CellKind::Grazer: ++counts.grazers; break;
        case CellKind::Predator: ++counts.predators; break;
    }
    return counts;
}
bool Ecosystem::valid() const noexcept {
    if(rng_==0U) return false;
    for(const auto& value:cells_) if(static_cast<unsigned>(value.kind)>static_cast<unsigned>(CellKind::Water)) return false;
    const auto p=population();
    return static_cast<unsigned>(p.grass)+p.water+p.plants+p.grazers+p.predators==static_cast<unsigned>(kCellCount);
}
} // namespace cosmos::world
