// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_GMXFILE_DEADCREATOR_H_
#define FLATBUFFERS_GENERATED_GMXFILE_DEADCREATOR_H_

#include "flatbuffers.h"

namespace DeadCreator {
    
    struct Vector2;
    
    struct Coord;
    
    struct Size;
    
    struct TileInfo;
    
    struct Entity;
    
    struct Polygon;
    
    struct Location;
    
    struct GMXFile;
    
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
    
    struct Location FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_NAME = 4,
            VT_POS = 6,
            VT_SIZE = 8
        };
        const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(VT_NAME); }
        const Vector2 *pos() const { return GetStruct<const Vector2 *>(VT_POS); }
        const Size *size() const { return GetStruct<const Size *>(VT_SIZE); }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_NAME) &&
            verifier.Verify(name()) &&
            VerifyField<Vector2>(verifier, VT_POS) &&
            VerifyField<Size>(verifier, VT_SIZE) &&
            verifier.EndTable();
        }
    };
    
    struct LocationBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(Location::VT_NAME, name); }
        void add_pos(const Vector2 *pos) { fbb_.AddStruct(Location::VT_POS, pos); }
        void add_size(const Size *size) { fbb_.AddStruct(Location::VT_SIZE, size); }
        LocationBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        LocationBuilder &operator=(const LocationBuilder &);
        flatbuffers::Offset<Location> Finish() {
            auto o = flatbuffers::Offset<Location>(fbb_.EndTable(start_, 3));
            return o;
        }
    };
    
    inline flatbuffers::Offset<Location> CreateLocation(flatbuffers::FlatBufferBuilder &_fbb,
                                                        flatbuffers::Offset<flatbuffers::String> name = 0,
                                                        const Vector2 *pos = 0,
                                                        const Size *size = 0) {
        LocationBuilder builder_(_fbb);
        builder_.add_size(size);
        builder_.add_pos(pos);
        builder_.add_name(name);
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
            VT_LOCATIONS = 18
        };
        TileType default_type() const { return static_cast<TileType>(GetField<int32_t>(VT_DEFAULT_TYPE, 0)); }
        const flatbuffers::Vector<flatbuffers::Offset<TileInfo>> *tiles() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<TileInfo>> *>(VT_TILES); }
        const Coord *number_of_tiles() const { return GetStruct<const Coord *>(VT_NUMBER_OF_TILES); }
        const Size *tile_size() const { return GetStruct<const Size *>(VT_TILE_SIZE); }
        const flatbuffers::Vector<flatbuffers::Offset<Polygon>> *collision_regions() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Polygon>> *>(VT_COLLISION_REGIONS); }
        const flatbuffers::Vector<flatbuffers::Offset<Entity>> *entities() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Entity>> *>(VT_ENTITIES); }
        const Size *cell_space_size() const { return GetStruct<const Size *>(VT_CELL_SPACE_SIZE); }
        const flatbuffers::Vector<flatbuffers::Offset<Location>> *locations() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Location>> *>(VT_LOCATIONS); }
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
        GMXFileBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
        GMXFileBuilder &operator=(const GMXFileBuilder &);
        flatbuffers::Offset<GMXFile> Finish() {
            auto o = flatbuffers::Offset<GMXFile>(fbb_.EndTable(start_, 8));
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
                                                      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Location>>> locations = 0) {
        GMXFileBuilder builder_(_fbb);
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
    
    inline const DeadCreator::GMXFile *GetGMXFile(const void *buf) { return flatbuffers::GetRoot<DeadCreator::GMXFile>(buf); }
    
    inline bool VerifyGMXFileBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<DeadCreator::GMXFile>(); }
    
    inline void FinishGMXFileBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<DeadCreator::GMXFile> root) { fbb.Finish(root); }
    
}  // namespace DeadCreator

#endif  // FLATBUFFERS_GENERATED_GMXFILE_DEADCREATOR_H_
