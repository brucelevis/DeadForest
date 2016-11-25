// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_PROFILINGSCHEMA_REALTRICK_PROFILER_H_
#define FLATBUFFERS_GENERATED_PROFILINGSCHEMA_REALTRICK_PROFILER_H_

#include "flatbuffers/flatbuffers.h"

namespace realtrick {
    namespace profiler {
        
        struct Element;
        
        struct Data;
        
        struct Element FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
            enum {
                VT_NAME = 4,
                VT_CALLS = 6,
                VT_AVG_TIME = 8,
                VT_MIN_TIME = 10,
                VT_MAX_TIME = 12,
                VT_USAGE = 14,
                VT_CHILDREN = 16
            };
            const flatbuffers::String *name() const { return GetPointer<const flatbuffers::String *>(VT_NAME); }
            int32_t calls() const { return GetField<int32_t>(VT_CALLS, 0); }
            int32_t avg_time() const { return GetField<int32_t>(VT_AVG_TIME, 0); }
            int32_t min_time() const { return GetField<int32_t>(VT_MIN_TIME, 0); }
            int32_t max_time() const { return GetField<int32_t>(VT_MAX_TIME, 0); }
            float usage() const { return GetField<float>(VT_USAGE, 0.0f); }
            const flatbuffers::Vector<flatbuffers::Offset<Element>> *children() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Element>> *>(VT_CHILDREN); }
            bool Verify(flatbuffers::Verifier &verifier) const {
                return VerifyTableStart(verifier) &&
                VerifyField<flatbuffers::uoffset_t>(verifier, VT_NAME) &&
                verifier.Verify(name()) &&
                VerifyField<int32_t>(verifier, VT_CALLS) &&
                VerifyField<int32_t>(verifier, VT_AVG_TIME) &&
                VerifyField<int32_t>(verifier, VT_MIN_TIME) &&
                VerifyField<int32_t>(verifier, VT_MAX_TIME) &&
                VerifyField<float>(verifier, VT_USAGE) &&
                VerifyField<flatbuffers::uoffset_t>(verifier, VT_CHILDREN) &&
                verifier.Verify(children()) &&
                verifier.VerifyVectorOfTables(children()) &&
                verifier.EndTable();
            }
        };
        
        struct ElementBuilder {
            flatbuffers::FlatBufferBuilder &fbb_;
            flatbuffers::uoffset_t start_;
            void add_name(flatbuffers::Offset<flatbuffers::String> name) { fbb_.AddOffset(Element::VT_NAME, name); }
            void add_calls(int32_t calls) { fbb_.AddElement<int32_t>(Element::VT_CALLS, calls, 0); }
            void add_avg_time(int32_t avg_time) { fbb_.AddElement<int32_t>(Element::VT_AVG_TIME, avg_time, 0); }
            void add_min_time(int32_t min_time) { fbb_.AddElement<int32_t>(Element::VT_MIN_TIME, min_time, 0); }
            void add_max_time(int32_t max_time) { fbb_.AddElement<int32_t>(Element::VT_MAX_TIME, max_time, 0); }
            void add_usage(float usage) { fbb_.AddElement<float>(Element::VT_USAGE, usage, 0.0f); }
            void add_children(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Element>>> children) { fbb_.AddOffset(Element::VT_CHILDREN, children); }
            ElementBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
            ElementBuilder &operator=(const ElementBuilder &);
            flatbuffers::Offset<Element> Finish() {
                auto o = flatbuffers::Offset<Element>(fbb_.EndTable(start_, 7));
                return o;
            }
        };
        
        inline flatbuffers::Offset<Element> CreateElement(flatbuffers::FlatBufferBuilder &_fbb,
                                                          flatbuffers::Offset<flatbuffers::String> name = 0,
                                                          int32_t calls = 0,
                                                          int32_t avg_time = 0,
                                                          int32_t min_time = 0,
                                                          int32_t max_time = 0,
                                                          float usage = 0.0f,
                                                          flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Element>>> children = 0) {
            ElementBuilder builder_(_fbb);
            builder_.add_children(children);
            builder_.add_usage(usage);
            builder_.add_max_time(max_time);
            builder_.add_min_time(min_time);
            builder_.add_avg_time(avg_time);
            builder_.add_calls(calls);
            builder_.add_name(name);
            return builder_.Finish();
        }
        
        struct Data FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
            enum {
                VT_MAIN_LOOP = 4,
                VT_TICK = 6,
                VT_FPS = 8
            };
            const Element *main_loop() const { return GetPointer<const Element *>(VT_MAIN_LOOP); }
            int32_t tick() const { return GetField<int32_t>(VT_TICK, 0); }
            float fps() const { return GetField<float>(VT_FPS, 0.0f); }
            bool Verify(flatbuffers::Verifier &verifier) const {
                return VerifyTableStart(verifier) &&
                VerifyField<flatbuffers::uoffset_t>(verifier, VT_MAIN_LOOP) &&
                verifier.VerifyTable(main_loop()) &&
                VerifyField<int32_t>(verifier, VT_TICK) &&
                VerifyField<float>(verifier, VT_FPS) &&
                verifier.EndTable();
            }
        };
        
        struct DataBuilder {
            flatbuffers::FlatBufferBuilder &fbb_;
            flatbuffers::uoffset_t start_;
            void add_main_loop(flatbuffers::Offset<Element> main_loop) { fbb_.AddOffset(Data::VT_MAIN_LOOP, main_loop); }
            void add_tick(int32_t tick) { fbb_.AddElement<int32_t>(Data::VT_TICK, tick, 0); }
            void add_fps(float fps) { fbb_.AddElement<float>(Data::VT_FPS, fps, 0.0f); }
            DataBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
            DataBuilder &operator=(const DataBuilder &);
            flatbuffers::Offset<Data> Finish() {
                auto o = flatbuffers::Offset<Data>(fbb_.EndTable(start_, 3));
                return o;
            }
        };
        
        inline flatbuffers::Offset<Data> CreateData(flatbuffers::FlatBufferBuilder &_fbb,
                                                    flatbuffers::Offset<Element> main_loop = 0,
                                                    int32_t tick = 0,
                                                    float fps = 0.0f) {
            DataBuilder builder_(_fbb);
            builder_.add_fps(fps);
            builder_.add_tick(tick);
            builder_.add_main_loop(main_loop);
            return builder_.Finish();
        }
        
        inline const realtrick::profiler::Data *GetData(const void *buf) { return flatbuffers::GetRoot<realtrick::profiler::Data>(buf); }
        
        inline bool VerifyDataBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<realtrick::profiler::Data>(); }
        
        inline void FinishDataBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<realtrick::profiler::Data> root) { fbb.Finish(root); }
        
    }  // namespace profiler
}  // namespace realtrick

#endif  // FLATBUFFERS_GENERATED_PROFILINGSCHEMA_REALTRICK_PROFILER_H_
