// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_PACKETSUCCESSSEARCHGAME_FPACKET_H_
#define FLATBUFFERS_GENERATED_PACKETSUCCESSSEARCHGAME_FPACKET_H_

#include "flatbuffers/flatbuffers.h"

namespace fpacket {

struct PacketSuccessSearchGame;

struct PacketSuccessSearchGame FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ROOMID = 4
  };
  int32_t roomId() const { return GetField<int32_t>(VT_ROOMID, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ROOMID) &&
           verifier.EndTable();
  }
};

struct PacketSuccessSearchGameBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_roomId(int32_t roomId) { fbb_.AddElement<int32_t>(PacketSuccessSearchGame::VT_ROOMID, roomId, 0); }
  PacketSuccessSearchGameBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  PacketSuccessSearchGameBuilder &operator=(const PacketSuccessSearchGameBuilder &);
  flatbuffers::Offset<PacketSuccessSearchGame> Finish() {
    auto o = flatbuffers::Offset<PacketSuccessSearchGame>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<PacketSuccessSearchGame> CreatePacketSuccessSearchGame(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t roomId = 0) {
  PacketSuccessSearchGameBuilder builder_(_fbb);
  builder_.add_roomId(roomId);
  return builder_.Finish();
}

inline const fpacket::PacketSuccessSearchGame *GetPacketSuccessSearchGame(const void *buf) { return flatbuffers::GetRoot<fpacket::PacketSuccessSearchGame>(buf); }

inline bool VerifyPacketSuccessSearchGameBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<fpacket::PacketSuccessSearchGame>(); }

inline void FinishPacketSuccessSearchGameBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<fpacket::PacketSuccessSearchGame> root) { fbb.Finish(root); }

}  // namespace fpacket

#endif  // FLATBUFFERS_GENERATED_PACKETSUCCESSSEARCHGAME_FPACKET_H_
