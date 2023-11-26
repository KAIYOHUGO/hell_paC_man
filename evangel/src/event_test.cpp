#include "event.h"
#include <gtest/gtest.h>

namespace bridge {
extern "C" void internal_event_queue_init(void);
}

enum Key {
  Key_W,
  Key_A,
  Key_S,
  Key_D,
};

enum Animation {
  Animation_Stop,
  Animation_Play,
};

DeclareEventType(Key);
DeclareEventType(Animation);

isize main(isize argc, char **argv) {
  bridge::internal_event_queue_init();
  add_event_type(Key);
  add_event_type(Animation);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

PEvent mock_to_ptr(void *e) { return PEvent{.self = e}; }

TEST(EventTest, EmitFlushAndListen) {
  Key *e = new Key;
  *e = Key_W;
  Key *e1 = new Key;
  *e1 = Key_A;
  Animation *e2 = new Animation;
  *e2 = Animation::Animation_Play;
  CEvent.emit(ETy(Key), mock_to_ptr(e));
  CEvent.emit(ETy(Key), mock_to_ptr(e1));
  CEvent.emit(ETy(Animation), mock_to_ptr(e2));
  EXPECT_EQ(CEvent.listen(ETy(Key))->len, 0);
  EXPECT_EQ(CEvent.listen(ETy(Animation))->len, 0);

  CEvent.flush();
  EXPECT_EQ(CEvent.listen(ETy(Key))->len, 2);
  EXPECT_EQ(CEvent.listen(ETy(Animation))->len, 1);

  CEvent.flush();
  EXPECT_EQ(CEvent.listen(ETy(Key))->len, 0);
  EXPECT_EQ(CEvent.listen(ETy(Animation))->len, 0);
  CEvent.flush();
}
