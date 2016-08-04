// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_GMXFILE_DEADCREATOR_H_
#define FLATBUFFERS_GENERATED_GMXFILE_DEADCREATOR_H_

#include "flatbuffers.h"

namespace DeadCreator {
    
    struct Vector2;
    
    struct Coord;
    
    struct Size;
    
    struct TileInfo;
    
    struct Location;
    
    struct Bring;
    
    struct DisplayText;
    
    struct Condition;
    
    struct Action;
    
    struct Trigger;
    
    struct Entity;
    
    struct Polygon;
    
    struct GMXFile;
    
    enum Approximation {
        Approximation_AtLeast = 0,
        Approximation_AtMost = 1,
        Approximation_Exactly = 2,
        Approximation_MIN = Approximation_AtLeast,
        Approximation_MAX = Approximation_Exactly
    };
    
    inline const char **EnumNamesApproximation() {
        static const char *names[] = { "AtLeast", "AtMost", "Exactly", nullptr };
        return names;
    }
    
    inline const char *EnumNameApproximation(Approximation e) { return EnumNamesApproximation()[static_cast<int>(e)]; }
    
    enum TileType {
        TileType_Dirt = 0,
        TileType_Grass = 1,
        TileType_Water = 2,
        TileType_Hill = 3,
        TileType_MIN = TileType_Dirt,
        TileType_MAX = TileType_Hill
    };
    
    inline const char **EnumNamesTileType() {
        static const char *names[] = { "Dirt", "Grass", "Water", "Hill", nullptr };
        return names;
    }
    
    inline const char *EnumNameTileType(TileType e) { return EnumNamesTileType()[static_cast<int>(e)]; }
    
    enum ConditionBase {
        ConditionBase_NONE = 0,
        ConditionBase_Bring = 1,
        ConditionBase_MIN = ConditionBase_NONE,
        ConditionBase_MAX = ConditionBase_Bring
    };
    
    inline const char **EnumNamesConditionBase() {
        static const char *names[] = { "NONE", "Bring", nullptr };
        return names;
    }
    
    inline const char *EnumNameConditionBase(ConditionBase e) { return EnumNamesConditionBase()[static_cast<int>(e)]; }
    
    inline bool VerifyConditionBase(flatbuffers::Verifier &verifier, const void *union_obj, ConditionBase type);
    
    enum ActionBase {
        ActionBase_NONE = 0,
        ActionBase_DisplayText = 1,
        ActionBase_MIN = ActionBase_NONE,
        ActionBase_MAX = ActionBase_DisplayText
    };
    
    inline const char **EnumNamesActionBase() {
        static const char *names[] = { "NONE", "DisplayText", nullptr };
        return names;
    }
    
    inline const char *EnumNameActionBase(ActionBase e) { return EnumNamesActionBase()[static_cast<int>(e)]; }
    
    inline bool VerifyActionBase(flatbuffers::Verifier &verifier, const void *union_obj, ActionBase type);
    
    MANUALLY_ALIGNED_STRUCT(4) Vector2 FLATBUFFERS_FINAL_CLASS {
    private:
        float x_;
        float y_;
        
    public:
        Vector2(float _x, float _y)
        : x_(flatbuffers::EndianScalar(_x)), y_(flatbuffers::EndianScalar(_y)) { }
        
        float x() const { return flatbuffers::EndianScalar(x_); }
        float y() const { return flatbuffers::EndianScalar(y_); }
    };
    STRUCT_END(Vector2, 8);
    
    MANUALLY_ALIGNED_STRUCT(4) Coord FLATBUFFERS_FINAL_CLASS {
    private:
        int32_t x_;
        int32_t y_;
        
    public:
        Coord(int32_t _x, int32_t _y)
        : x_(flatbuffers::EndianScalar(_x)), y_(flatbuffers::EndianScalar(_y)) { }
        
        int32_t x() const { return flatbuffers::EndianScalar(x_); }
        int32_t y() const { return flatbuffers::EndianScalar(y_); }
    };
    STRUCT_END(Coord, 8);
    
    MANUALLY_ALIGNED_STRUCT(4) Size FLATBUFFERS_FINAL_CLASS {
    private:
        int32_t width_;
        int32_t height_;
        
    public:
        Size(int32_t _width, int32_t _height)
        : width_(flatbuffers::EndianScalar(_width)), height_(flatbuffers::EndianScalar(_height)) { }
        
        int32_t width() const { return flatbuffers::EndianScalar(width_); }
        int32_t height() const { return flatbuffers::EndianScalar(height_); }
    };
    STRUCT_END(Size, 8);
    
    struct TileInfo FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_NUMBER = 4,
            VT_INDICES = 6
        };
        const flatbuffers::String *number() const { return GetPointer<const flatbuffers::String *>(VT_NUMBER); }
        const Coord *indices() const { return GetStruct<const Coord *>(VT_INDICES); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_NUMBER) &&
            verifier.Verify(number()) &&
            VerifyField<Coord>(verifier, VT_INDICES) &&
            verifier.EndTable();
        }
    };
    
    struct TileInfoBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_number(flatbuffers::Offset<flatbuffers::String> number) { fbb_.AddOffset(TileInfo::VT_NUMBER, number); }
        void add_indices(const Coord *indices) { fbb_.AddStruct(TileInfo::VT_INDICES, indices); }
        TileInfoBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        TileInfoBuilder &operator=(const TileInfoBuilder &);
        flatbuffers::Offset<TileInfo> Finish() {
            auto o = flatbuffers::Offset<TileInfo>(fbb_.EndTable(start_, 2));
            return o;
        }
    };
    
    inline flatbuffers::Offset<TileInfo> CreateTileInfo(flatbuffers::FlatBufferBuilder &_fbb,
                                                        flatbuffers::Offset<flatbuffers::String> number = 0,
                                                        const Coord *indices = 0) {
        TileInfoBuilder builder_(_fbb);
        builder_.add_indices(indices);
        builder_.add_number(number);
        return builder_.Finish();
    }
    
    struct Location FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_NAME = 4,
            VT_SIZE = 6,
            VT_POS = 8
        };
        const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(VT_NAME); }
        const Size *size() const { return GetStruct<const Size *>(VT_SIZE); }
        const Vector2 *pos() const { return GetStruct<const Vector2 *>(VT_POS); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_NAME) &&
            verifier.Verify(name()) &&
            VerifyField<Size>(verifier, VT_SIZE) &&
            VerifyField<Vector2>(verifier, VT_POS) &&
            verifier.EndTable();
        }
    };
    
    struct LocationBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(Location::VT_NAME, name); }
        void add_size(const Size *size) { fbb_.AddStruct(Location::VT_SIZE, size); }
        void add_pos(const Vector2 *pos) { fbb_.AddStruct(Location::VT_POS, pos); }
        LocationBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        LocationBuilder &operator=(const LocationBuilder &);
        flatbuffers::Offset<Location> Finish() {
            auto o = flatbuffers::Offset<Location>(fbb_.EndTable(start_, 3));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Location> CreateLocation(flatbuffers::FlatBufferBuilder &_fbb,
                                                        flatbuffers::Offset<flatbuffers::String> name = 0,
                                                        const Size *size = 0,
                                                        const Vector2 *pos = 0) {
        LocationBuilder builder_(_fbb);
        builder_.add_pos(pos);
        builder_.add_size(size);
        builder_.add_name(name);
        return builder_.Finish();
    }
    
    struct Bring FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_PLAYER = 4,
            VT_APPROXIMATION = 6,
            VT_NUMBER = 8,
            VT_ENTITY_TYPE = 10,
            VT_LOCATION_NAME = 12
        };
        int32_t player() const { return GetField<int32_t>(VT_PLAYER, 0); }
        Approximation approximation() const { return static_cast<Approximation>(GetField<int32_t>(VT_APPROXIMATION, 0)); }
        int32_t number() const { return GetField<int32_t>(VT_NUMBER, 0); }
        int32_t entity_type() const { return GetField<int32_t>(VT_ENTITY_TYPE, 0); }
        const flatbuffers::String *location_name() const { return GetPointer<const flatbuffers::String *>(VT_LOCATION_NAME); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<int32_t>(verifier, VT_PLAYER) &&
            VerifyField<int32_t>(verifier, VT_APPROXIMATION) &&
            VerifyField<int32_t>(verifier, VT_NUMBER) &&
            VerifyField<int32_t>(verifier, VT_ENTITY_TYPE) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_LOCATION_NAME) &&
            verifier.Verify(location_name()) &&
            verifier.EndTable();
        }
    };
    
    struct BringBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_player(int32_t player) { fbb_.AddElement<int32_t>(Bring::VT_PLAYER, player, 0); }
        void add_approximation(Approximation approximation) { fbb_.AddElement<int32_t>(Bring::VT_APPROXIMATION, static_cast<int32_t>(approximation), 0); }
        void add_number(int32_t number) { fbb_.AddElement<int32_t>(Bring::VT_NUMBER, number, 0); }
        void add_entity_type(int32_t entity_type) { fbb_.AddElement<int32_t>(Bring::VT_ENTITY_TYPE, entity_type, 0); }
        void add_location_name(flatbuffers::Offset<flatbuffers::String> location_name) { fbb_.AddOffset(Bring::VT_LOCATION_NAME, location_name); }
        BringBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        BringBuilder &operator=(const BringBuilder &);
        flatbuffers::Offset<Bring> Finish() {
            auto o = flatbuffers::Offset<Bring>(fbb_.EndTable(start_, 5));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Bring> CreateBring(flatbuffers::FlatBufferBuilder &_fbb,
                                                  int32_t player = 0,
                                                  Approximation approximation = Approximation_AtLeast,
                                                  int32_t number = 0,
                                                  int32_t entity_type = 0,
                                                  flatbuffers::Offset<flatbuffers::String> location_name = 0) {
        BringBuilder builder_(_fbb);
        builder_.add_location_name(location_name);
        builder_.add_entity_type(entity_type);
        builder_.add_number(number);
        builder_.add_approximation(approximation);
        builder_.add_player(player);
        return builder_.Finish();
    }
    
    struct DisplayText FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_TEXT = 4
        };
        const flatbuffers::String *text() const { return GetPointer<const flatbuffers::String *>(VT_TEXT); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT) &&
            verifier.Verify(text()) &&
            verifier.EndTable();
        }
    };
    
    struct DisplayTextBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_text(flatbuffers::Offset<flatbuffers::String> text) { fbb_.AddOffset(DisplayText::VT_TEXT, text); }
        DisplayTextBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        DisplayTextBuilder &operator=(const DisplayTextBuilder &);
        flatbuffers::Offset<DisplayText> Finish() {
            auto o = flatbuffers::Offset<DisplayText>(fbb_.EndTable(start_, 1));
            return o;
        }
    };
    
    inline flatbuffers::Offset<DisplayText> CreateDisplayText(flatbuffers::FlatBufferBuilder &_fbb,
                                                              flatbuffers::Offset<flatbuffers::String> text = 0) {
        DisplayTextBuilder builder_(_fbb);
        builder_.add_text(text);
        return builder_.Finish();
    }
    
    struct Condition FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_CONDITION_TYPE = 4,
            VT_CONDITION = 6
        };
        ConditionBase condition_type() const { return static_cast<ConditionBase>(GetField<uint8_t>(VT_CONDITION_TYPE, 0)); }
        const void *condition() const { return GetPointer<const void *>(VT_CONDITION); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<uint8_t>(verifier, VT_CONDITION_TYPE) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_CONDITION) &&
            VerifyConditionBase(verifier, condition(), condition_type()) &&
            verifier.EndTable();
        }
    };
    
    struct ConditionBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_condition_type(ConditionBase condition_type) { fbb_.AddElement<uint8_t>(Condition::VT_CONDITION_TYPE, static_cast<uint8_t>(condition_type), 0); }
        void add_condition(flatbuffers::Offset<void> condition) { fbb_.AddOffset(Condition::VT_CONDITION, condition); }
        ConditionBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        ConditionBuilder &operator=(const ConditionBuilder &);
        flatbuffers::Offset<Condition> Finish() {
            auto o = flatbuffers::Offset<Condition>(fbb_.EndTable(start_, 2));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Condition> CreateCondition(flatbuffers::FlatBufferBuilder &_fbb,
                                                          ConditionBase condition_type = ConditionBase_NONE,
                                                          flatbuffers::Offset<void> condition = 0) {
        ConditionBuilder builder_(_fbb);
        builder_.add_condition(condition);
        builder_.add_condition_type(condition_type);
        return builder_.Finish();
    }
    
    struct Action FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_ACTION_TYPE = 4,
            VT_ACTION = 6
        };
        ActionBase action_type() const { return static_cast<ActionBase>(GetField<uint8_t>(VT_ACTION_TYPE, 0)); }
        const void *action() const { return GetPointer<const void *>(VT_ACTION); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<uint8_t>(verifier, VT_ACTION_TYPE) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_ACTION) &&
            VerifyActionBase(verifier, action(), action_type()) &&
            verifier.EndTable();
        }
    };
    
    struct ActionBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_action_type(ActionBase action_type) { fbb_.AddElement<uint8_t>(Action::VT_ACTION_TYPE, static_cast<uint8_t>(action_type), 0); }
        void add_action(flatbuffers::Offset<void> action) { fbb_.AddOffset(Action::VT_ACTION, action); }
        ActionBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        ActionBuilder &operator=(const ActionBuilder &);
        flatbuffers::Offset<Action> Finish() {
            auto o = flatbuffers::Offset<Action>(fbb_.EndTable(start_, 2));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Action> CreateAction(flatbuffers::FlatBufferBuilder &_fbb,
                                                    ActionBase action_type = ActionBase_NONE,
                                                    flatbuffers::Offset<void> action = 0) {
        ActionBuilder builder_(_fbb);
        builder_.add_action(action);
        builder_.add_action_type(action_type);
        return builder_.Finish();
    }
    
    struct Trigger FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_PLAYERS = 4,
            VT_CONDITIONS = 6,
            VT_ACTIONS = 8
        };
        const flatbuffers::Vector<int32_t> *players() const { return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_PLAYERS); }
        const flatbuffers::Vector<flatbuffers::Offset<Condition>> *conditions() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Condition>> *>(VT_CONDITIONS); }
        const flatbuffers::Vector<flatbuffers::Offset<Action>> *actions() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Action>> *>(VT_ACTIONS); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_PLAYERS) &&
            verifier.Verify(players()) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_CONDITIONS) &&
            verifier.Verify(conditions()) &&
            verifier.VerifyVectorOfTables(conditions()) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_ACTIONS) &&
            verifier.Verify(actions()) &&
            verifier.VerifyVectorOfTables(actions()) &&
            verifier.EndTable();
        }
    };
    
    struct TriggerBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_players(flatbuffers::Offset<flatbuffers::Vector<int32_t>> players) { fbb_.AddOffset(Trigger::VT_PLAYERS, players); }
        void add_conditions(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Condition>>> conditions) { fbb_.AddOffset(Trigger::VT_CONDITIONS, conditions); }
        void add_actions(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Action>>> actions) { fbb_.AddOffset(Trigger::VT_ACTIONS, actions); }
        TriggerBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        TriggerBuilder &operator=(const TriggerBuilder &);
        flatbuffers::Offset<Trigger> Finish() {
            auto o = flatbuffers::Offset<Trigger>(fbb_.EndTable(start_, 3));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Trigger> CreateTrigger(flatbuffers::FlatBufferBuilder &_fbb,
                                                      flatbuffers::Offset<flatbuffers::Vector<int32_t>> players = 0,
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Condition>>> conditions = 0,
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Action>>> actions = 0) {
        TriggerBuilder builder_(_fbb);
        builder_.add_actions(actions);
        builder_.add_conditions(conditions);
        builder_.add_players(players);
        return builder_.Finish();
    }
    
    struct Entity FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_PLAYER_TYPE = 4,
            VT_ENTITY_TYPE = 6,
            VT_POS = 8
        };
        int32_t player_type() const { return GetField<int32_t>(VT_PLAYER_TYPE, 0); }
        int32_t entity_type() const { return GetField<int32_t>(VT_ENTITY_TYPE, 0); }
        const Vector2 *pos() const { return GetStruct<const Vector2 *>(VT_POS); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<int32_t>(verifier, VT_PLAYER_TYPE) &&
            VerifyField<int32_t>(verifier, VT_ENTITY_TYPE) &&
            VerifyField<Vector2>(verifier, VT_POS) &&
            verifier.EndTable();
        }
    };
    
    struct EntityBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_player_type(int32_t player_type) { fbb_.AddElement<int32_t>(Entity::VT_PLAYER_TYPE, player_type, 0); }
        void add_entity_type(int32_t entity_type) { fbb_.AddElement<int32_t>(Entity::VT_ENTITY_TYPE, entity_type, 0); }
        void add_pos(const Vector2 *pos) { fbb_.AddStruct(Entity::VT_POS, pos); }
        EntityBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        EntityBuilder &operator=(const EntityBuilder &);
        flatbuffers::Offset<Entity> Finish() {
            auto o = flatbuffers::Offset<Entity>(fbb_.EndTable(start_, 3));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Entity> CreateEntity(flatbuffers::FlatBufferBuilder &_fbb,
                                                    int32_t player_type = 0,
                                                    int32_t entity_type = 0,
                                                    const Vector2 *pos = 0) {
        EntityBuilder builder_(_fbb);
        builder_.add_pos(pos);
        builder_.add_entity_type(entity_type);
        builder_.add_player_type(player_type);
        return builder_.Finish();
    }
    
    struct Polygon FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_VERTICES = 4
        };
        const flatbuffers::Vector<const Vector2 *> *vertices() const { return GetPointer<const flatbuffers::Vector<const Vector2 *> *>(VT_VERTICES); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_VERTICES) &&
            verifier.Verify(vertices()) &&
            verifier.EndTable();
        }
    };
    
    struct PolygonBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_vertices(flatbuffers::Offset<flatbuffers::Vector<const Vector2 *>> vertices) { fbb_.AddOffset(Polygon::VT_VERTICES, vertices); }
        PolygonBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        PolygonBuilder &operator=(const PolygonBuilder &);
        flatbuffers::Offset<Polygon> Finish() {
            auto o = flatbuffers::Offset<Polygon>(fbb_.EndTable(start_, 1));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Polygon> CreatePolygon(flatbuffers::FlatBufferBuilder &_fbb,
                                                      flatbuffers::Offset<flatbuffers::Vector<const Vector2 *>> vertices = 0) {
        PolygonBuilder builder_(_fbb);
        builder_.add_vertices(vertices);
        return builder_.Finish();
    }
    
    struct GMXFile FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_DEFAULT_TYPE = 4,
            VT_TILES = 6,
            VT_NUMBER_OF_TILES = 8,
            VT_TILE_SIZE = 10,
            VT_COLLISION_REGIONS = 12,
            VT_ENTITIES = 14,
            VT_CELL_SPACE_SIZE = 16,
            VT_LOCATIONS = 18,
            VT_TRIGGERS = 20
        };
        TileType default_type() const { return static_cast<TileType>(GetField<int32_t>(VT_DEFAULT_TYPE, 0)); }
        const flatbuffers::Vector<flatbuffers::Offset<TileInfo>> *tiles() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<TileInfo>> *>(VT_TILES); }
        const Coord *number_of_tiles() const { return GetStruct<const Coord *>(VT_NUMBER_OF_TILES); }
        const Size *tile_size() const { return GetStruct<const Size *>(VT_TILE_SIZE); }
        const flatbuffers::Vector<flatbuffers::Offset<Polygon>> *collision_regions() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Polygon>> *>(VT_COLLISION_REGIONS); }
        const flatbuffers::Vector<flatbuffers::Offset<Entity>> *entities() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Entity>> *>(VT_ENTITIES); }
        const Size *cell_space_size() const { return GetStruct<const Size *>(VT_CELL_SPACE_SIZE); }
        const flatbuffers::Vector<flatbuffers::Offset<Location>> *locations() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Location>> *>(VT_LOCATIONS); }
        const flatbuffers::Vector<flatbuffers::Offset<Trigger>> *triggers() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Trigger>> *>(VT_TRIGGERS); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<int32_t>(verifier, VT_DEFAULT_TYPE) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_TILES) &&
            verifier.Verify(tiles()) &&
            verifier.VerifyVectorOfTables(tiles()) &&
            VerifyField<Coord>(verifier, VT_NUMBER_OF_TILES) &&
            VerifyField<Size>(verifier, VT_TILE_SIZE) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_COLLISION_REGIONS) &&
            verifier.Verify(collision_regions()) &&
            verifier.VerifyVectorOfTables(collision_regions()) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_ENTITIES) &&
            verifier.Verify(entities()) &&
            verifier.VerifyVectorOfTables(entities()) &&
            VerifyField<Size>(verifier, VT_CELL_SPACE_SIZE) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_LOCATIONS) &&
            verifier.Verify(locations()) &&
            verifier.VerifyVectorOfTables(locations()) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_TRIGGERS) &&
            verifier.Verify(triggers()) &&
            verifier.VerifyVectorOfTables(triggers()) &&
            verifier.EndTable();
        }
    };
    
    struct GMXFileBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_default_type(TileType default_type) { fbb_.AddElement<int32_t>(GMXFile::VT_DEFAULT_TYPE, static_cast<int32_t>(default_type), 0); }
        void add_tiles(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TileInfo>>> tiles) { fbb_.AddOffset(GMXFile::VT_TILES, tiles); }
        void add_number_of_tiles(const Coord *number_of_tiles) { fbb_.AddStruct(GMXFile::VT_NUMBER_OF_TILES, number_of_tiles); }
        void add_tile_size(const Size *tile_size) { fbb_.AddStruct(GMXFile::VT_TILE_SIZE, tile_size); }
        void add_collision_regions(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Polygon>>> collision_regions) { fbb_.AddOffset(GMXFile::VT_COLLISION_REGIONS, collision_regions); }
        void add_entities(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Entity>>> entities) { fbb_.AddOffset(GMXFile::VT_ENTITIES, entities); }
        void add_cell_space_size(const Size *cell_space_size) { fbb_.AddStruct(GMXFile::VT_CELL_SPACE_SIZE, cell_space_size); }
        void add_locations(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Location>>> locations) { fbb_.AddOffset(GMXFile::VT_LOCATIONS, locations); }
        void add_triggers(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Trigger>>> triggers) { fbb_.AddOffset(GMXFile::VT_TRIGGERS, triggers); }
        GMXFileBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        GMXFileBuilder &operator=(const GMXFileBuilder &);
        flatbuffers::Offset<GMXFile> Finish() {
            auto o = flatbuffers::Offset<GMXFile>(fbb_.EndTable(start_, 9));
            return o;
        }
    };
    
    inline flatbuffers::Offset<GMXFile> CreateGMXFile(flatbuffers::FlatBufferBuilder &_fbb,
                                                      TileType default_type = TileType_Dirt,
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TileInfo>>> tiles = 0,
                                                      const Coord *number_of_tiles = 0,
                                                      const Size *tile_size = 0,
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Polygon>>> collision_regions = 0,
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Entity>>> entities = 0,
                                                      const Size *cell_space_size = 0,
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Location>>> locations = 0,
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Trigger>>> triggers = 0) {
        GMXFileBuilder builder_(_fbb);
        builder_.add_triggers(triggers);
        builder_.add_locations(locations);
        builder_.add_cell_space_size(cell_space_size);
        builder_.add_entities(entities);
        builder_.add_collision_regions(collision_regions);
        builder_.add_tile_size(tile_size);
        builder_.add_number_of_tiles(number_of_tiles);
        builder_.add_tiles(tiles);
        builder_.add_default_type(default_type);
        return builder_.Finish();
    }
    
    inline bool VerifyConditionBase(flatbuffers::Verifier &verifier, const void *union_obj, ConditionBase type) {
        switch (type) {
            case ConditionBase_NONE: return true;
            case ConditionBase_Bring: return verifier.VerifyTable(reinterpret_cast<const Bring *>(union_obj));
            default: return false;
        }
    }
    
    inline bool VerifyActionBase(flatbuffers::Verifier &verifier, const void *union_obj, ActionBase type) {
        switch (type) {
            case ActionBase_NONE: return true;
            case ActionBase_DisplayText: return verifier.VerifyTable(reinterpret_cast<const DisplayText *>(union_obj));
            default: return false;
        }
    }
    
    inline const DeadCreator::GMXFile *GetGMXFile(const void *buf) { return flatbuffers::GetRoot<DeadCreator::GMXFile>(buf); }
    
    inline bool VerifyGMXFileBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<DeadCreator::GMXFile>(); }
    
    inline void FinishGMXFileBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<DeadCreator::GMXFile> root) { fbb.Finish(root); }
    
}  // namespace DeadCreator

#endif  // FLATBUFFERS_GENERATED_GMXFILE_DEADCREATOR_H_
