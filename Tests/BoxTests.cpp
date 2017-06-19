#include <Box/Box.hpp>
#include <Box/Event.hpp>
#include <Box/EventPublisher.hpp>
#include <Box/EventForwarder.hpp>
#include <Box/MouseState.hpp>
#include <Stick/Test.hpp>

using namespace stick;
using namespace box;

struct TestEvent : public EventT<TestEvent> {};

static bool bWasCalled = false;
static void freeFunctionCallback(const TestEvent & _evt)
{
    bWasCalled = true;
}

struct TestClass
{
    void memberCallback(const TestEvent & _evt)
    {
        bWasCalled = true;
    }

    bool bWasCalled = false;
};

const Suite spec[] =
{
    // SUITE("Hierarchy Tests")
    // {
    //     auto root = createNode();
    //     auto a = createNode();
    //     auto b = createNode();
    //     auto c = createNode();
    //     addChild(root, a);
    //     addChild(root, b);
    //     addChild(b, c);

    //     EXPECT(a.get<comps::Parent>() == root);
    //     EXPECT(b.get<comps::Parent>() == root);

    //     EXPECT(root.get<comps::Children>().count() == 2);
    //     EXPECT(root.get<comps::Children>()[0] == a);
    //     EXPECT(root.get<comps::Children>()[1] == b);

    //     reverseChildren(root);
    //     EXPECT(root.get<comps::Children>()[0] == b);
    //     EXPECT(root.get<comps::Children>()[1] == a);

    //     removeChild(root, b);
    //     EXPECT(root.get<comps::Children>().count() == 1);
    //     EXPECT(root.get<comps::Children>()[0] == a);
    //     EXPECT(!b.hasComponent<comps::Parent>());

    //     remove(a);
    //     EXPECT(root.get<comps::Children>().count() == 0);

    //     EXPECT(c.isValid());
    //     remove(b);
    //     EXPECT(!c.isValid());
    // },
    // SUITE("Basic Tests")
    // {
    //     auto root = createNode();
    //     root.set<comps::Width>(100.0f);
    //     EXPECT(root.get<comps::Width>().value == 100.0f);
    //     EXPECT(root.get<comps::Width>().unit == Unit::Pixels);

    //     root.set<comps::Height>(50.0f, Unit::Percent);
    //     EXPECT(root.get<comps::Height>().value == 50.0f);
    //     EXPECT(root.get<comps::Height>().unit == Unit::Percent);

    //     root.set<comps::MaxWidth>(200.0f);
    //     EXPECT(root.get<comps::MaxWidth>().value == 200.0f);
    //     root.set<comps::MaxHeight>(55.3f);
    //     EXPECT(root.get<comps::MaxHeight>().value == 55.3f);

    //     root.set<comps::MinWidth>(33.0f);
    //     EXPECT(root.get<comps::MinWidth>().value == 33.0f);
    //     root.set<comps::MinHeight>(75.99f);
    //     EXPECT(root.get<comps::MinHeight>().value == 75.99f);

    //     root.set<comps::Left>(1.0f);
    //     EXPECT(root.get<comps::Left>().value == 1.0f);
    //     root.set<comps::Right>(2.0f);
    //     EXPECT(root.get<comps::Right>().value == 2.0f);

    //     root.set<comps::Top>(3.0f);
    //     EXPECT(root.get<comps::Top>().value == 3.0f);
    //     root.set<comps::Bottom>(4.0f);
    //     EXPECT(root.get<comps::Bottom>().value == 4.0f);

    //     root.set<comps::Overflow>(Overflow::Scroll);
    //     EXPECT(root.get<comps::Overflow>() == Overflow::Scroll);

    //     root.set<comps::Flow>(Flow::Column);
    //     EXPECT(root.get<comps::Flow>() == Flow::Column);

    //     root.set<comps::Direction>(Direction::LeftToRight);
    //     EXPECT(root.get<comps::Direction>() == Direction::LeftToRight);

    //     root.set<comps::Wrap>(Wrap::Reverse);
    //     EXPECT(root.get<comps::Wrap>() == Wrap::Reverse);

    //     root.set<comps::Justify>(Justify::SpaceBetween);
    //     EXPECT(root.get<comps::Justify>() == Justify::SpaceBetween);

    //     root.set<comps::Align>(Align::Stretch);
    //     EXPECT(root.get<comps::Align>() == Align::Stretch);

    //     root.set<comps::Position>(Position::Absolute);
    //     EXPECT(root.get<comps::Position>() == Position::Absolute);
    // },
    // SUITE("Basic Layout Tests")
    // {
    //     auto root = createNode("Root");
    //     root.set<comps::Width>(800.0f);
    //     root.set<comps::Height>(600.0f);
    //     auto a = createNode("A");
    //     addChild(root, a);
    //     auto b = createNode("B");
    //     b.set<comps::Width>(400.0f);
    //     b.set<comps::MaxWidth>(200.0f);
    //     b.set<comps::Height>(300.0f);
    //     addChild(a, b);
    //     auto c = createNode("C");
    //     c.set<comps::Width>(200.0f);
    //     c.set<comps::Height>(200.0f);
    //     addChild(a, c);

    //     auto d = createNode("d");
    //     d.set<comps::Width>(100.0f, Unit::Percent);
    //     d.set<comps::Height>(100.0f);
    //     addChild(root, d);

    //     Error err = layout(root);
    //     EXPECT(!err);
    //     EXPECT(root.hasComponent<comps::ComputedLayout>());
    //     EXPECT(root.get<comps::ComputedLayout>().box.min().x == 0);
    //     EXPECT(root.get<comps::ComputedLayout>().box.min().y == 0);
    //     EXPECT(root.get<comps::ComputedLayout>().box.width() == 800.0f);
    //     EXPECT(root.get<comps::ComputedLayout>().box.height() == 600.0f);

    //     EXPECT(a.hasComponent<comps::ComputedLayout>());
    //     EXPECT(a.get<comps::ComputedLayout>().box.min().x == 0);
    //     EXPECT(a.get<comps::ComputedLayout>().box.min().y == 0);
    //     EXPECT(a.get<comps::ComputedLayout>().box.width() == 400.0f);
    //     EXPECT(a.get<comps::ComputedLayout>().box.height() == 600.0f);

    //     EXPECT(b.hasComponent<comps::ComputedLayout>());
    //     EXPECT(b.get<comps::ComputedLayout>().box.min().x == 0);
    //     EXPECT(b.get<comps::ComputedLayout>().box.min().y == 0);
    //     EXPECT(b.get<comps::ComputedLayout>().box.width() == 200.0f);
    //     EXPECT(b.get<comps::ComputedLayout>().box.height() == 300.0f);

    //     EXPECT(c.hasComponent<comps::ComputedLayout>());
    //     EXPECT(c.get<comps::ComputedLayout>().box.min().x == 200.0f);
    //     EXPECT(c.get<comps::ComputedLayout>().box.min().y == 0);
    //     EXPECT(c.get<comps::ComputedLayout>().box.width() == 200.0f);
    //     EXPECT(c.get<comps::ComputedLayout>().box.height() == 300.0f);

    //     EXPECT(d.hasComponent<comps::ComputedLayout>());
    //     EXPECT(d.get<comps::ComputedLayout>().box.min().x == 0.0);
    //     EXPECT(d.get<comps::ComputedLayout>().box.min().y == 600.0f);
    //     EXPECT(d.get<comps::ComputedLayout>().box.width() == 800.0f);
    //     EXPECT(d.get<comps::ComputedLayout>().box.height() == 100.0f);

    //     printf("%f %f %f %f\n", d.get<comps::ComputedLayout>().box.min().x, d.get<comps::ComputedLayout>().box.min().y, d.get<comps::ComputedLayout>().box.width(), d.get<comps::ComputedLayout>().box.height());
    // },
    SUITE("Callback Tests")
    {
        //@Note: This is not really testing much but rather checking if
        //things compile as expected :)
        using Callback = box::detail::CallbackT<void, box::Event>;

        Callback cb(&freeFunctionCallback);

        TestClass tc;
        Callback cb2(&tc, &TestClass::memberCallback);


        bool bLamdaCalled = false;
        Callback cb3([&](const TestEvent & _evt) { bLamdaCalled = true; });

        cb.call(TestEvent());
        EXPECT(bWasCalled);

        cb2.call(TestEvent());
        EXPECT(tc.bWasCalled);

        cb3.call(TestEvent());
        EXPECT(bLamdaCalled);
    },
    SUITE("EventPublisher Tests")
    {
        bWasCalled = false;
        EventPublisher publisher;

        publisher.addEventCallback(&freeFunctionCallback);
        TestClass tc;
        publisher.addEventCallback(EventPublisher::Callback(&tc, &TestClass::memberCallback));

        bool bLamdaCalled = false;
        publisher.addEventCallback([&](const TestEvent & _evt) { bLamdaCalled = true; });

        publisher.publish(TestEvent());
        EXPECT(bWasCalled);
        EXPECT(tc.bWasCalled);
        EXPECT(bLamdaCalled);
    },
    SUITE("EventForwarder Tests")
    {
        bWasCalled = false;
        EventForwarder publisher;

        publisher.addEventCallback(&freeFunctionCallback);
        TestClass tc;
        publisher.addEventCallback(EventForwarder::Callback(&tc, &TestClass::memberCallback));

        bool bLamdaCalled = false;
        publisher.addEventCallback([&](const TestEvent & _evt) { bLamdaCalled = true; });

        publisher.publish(TestEvent());
        EXPECT(bWasCalled);
        EXPECT(tc.bWasCalled);
        EXPECT(bLamdaCalled);
    }
};

int main(int _argc, const char * _args[])
{
    return runTests(spec, _argc, _args);
}
