#include <cxxtest/TestDrive.h>

#include <openma/io/handlerwriter.h>
#include <openma/io/file.h>

#include "c3dhandlerTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(C3DWriterTest)
{
  CXXTEST_TEST(capability)
  {
    ma::io::HandlerWriter writer(nullptr,"org.c3d");
    TS_ASSERT_EQUALS(writer.canWrite(), false);
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/sample01_Eb015pi.c3d"), ma::io::Mode::Out);
    writer.setDevice(&file);
    TS_ASSERT_EQUALS(writer.canWrite(), true);
    TS_ASSERT_EQUALS(writer.format(), "org.c3d");
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(supportedTrialFormats)
  {
    ma::io::HandlerWriter writer;
    auto formats = writer.availableFormats();
    TS_ASSERT_EQUALS(std::find(formats.cbegin(), formats.cend(), "org.c3d") != formats.cend(), true);
  };

  CXXTEST_TEST(detectNoDevice)
  {
    ma::io::HandlerWriter writer;
    TS_ASSERT_EQUALS(writer.canWrite(), false);
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::Device);
  };

  CXXTEST_TEST(detectDeviceNotOpen)
  {
    ma::io::File file;
    ma::io::HandlerWriter writer(&file);
    TS_ASSERT_EQUALS(writer.canWrite(), false);
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::Device);
  };

  CXXTEST_TEST(detectFail)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("fail.xyz"), ma::io::Mode::Out);
    ma::io::HandlerWriter writer(&file);
    TS_ASSERT_EQUALS(writer.canWrite(), false);
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::UnsupportedFormat);
  };

  CXXTEST_TEST(detectOk)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/sample01_Eb015pi.c3d"), ma::io::Mode::Out);
    ma::io::HandlerWriter writer(&file);
    TS_ASSERT_EQUALS(writer.canWrite(), true);
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::None);
  };

  CXXTEST_TEST(queryOkOne)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/sample01_Eb015pi.c3d"), ma::io::Mode::Out);
    ma::io::HandlerWriter writer(&file, "org.c3d");
    TS_ASSERT_EQUALS(writer.canWrite(), true);
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::None);
  };

  CXXTEST_TEST(queryOkTwo)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/sample01_Eb015pi.c3d"), ma::io::Mode::Out);
    ma::io::HandlerWriter writer(nullptr, "org.c3d");
    writer.setDevice(&file);
    TS_ASSERT_EQUALS(writer.canWrite(), true);
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::None);
  };

  CXXTEST_TEST(queryOkThree)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/sample01_Eb015pi.c3d"), ma::io::Mode::Out);
    ma::io::HandlerWriter writer;
    writer.setDevice(&file);
    writer.setFormat("org.c3d");
    TS_ASSERT_EQUALS(writer.canWrite(), true);
    TS_ASSERT_EQUALS(writer.errorCode(), ma::io::Error::None);
  };

  CXXTEST_TEST(sample01Rewrited)
  {
    c3dhandlertest_rewrite_sample01("PI", "sample01_Eb015pi.c3d", OPENMA_TDD_PATH_OUT("c3d/sample01_Eb015pi.c3d"), OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"));
  }

  CXXTEST_TEST(sample09Rewrited)
  {
    ma::Node rootIn("rootIn"), rootOut("rootOut");
    if (!c3dhandlertest_read("", OPENMA_TDD_PATH_IN("c3d/standard/sample09/PlugInC3D.c3d"), &rootIn)) return;
    ma::TimeSequence* ts = rootIn.findChild<ma::TimeSequence*>("LHipForce");
    TS_ASSERT_EQUALS(ts->type(), ma::TimeSequence::Force);
    TS_ASSERT_EQUALS(ts->unit(), "N");
    if (!c3dhandlertest_write("", OPENMA_TDD_PATH_OUT("c3d/sample09_PlugInC3D.c3d"), &rootIn)) return;
    if (!c3dhandlertest_read("", OPENMA_TDD_PATH_OUT("c3d/sample09_PlugInC3D.c3d"), &rootOut)) return;
    ma::TimeSequence* ts2 = rootOut.findChild<ma::TimeSequence*>("LHipForce");
    TS_ASSERT_EQUALS(ts2->type(), ma::TimeSequence::Force);
    TS_ASSERT_EQUALS(ts2->unit(), "N");
    // Compare time sequences together
    TS_ASSERT_EQUALS(ts2->elements(), ts->elements());
    TS_ASSERT_EQUALS(ts2->elements(), size_t(281 * 4));
    for (unsigned i = 0 ; i < ts->elements() ; ++i)
    {
      TS_ASSERT_DELTA(ts2->data()[i], ts->data()[i], 1e-5);
    }
    TS_ASSERT_EQUALS(rootIn.child(0)->property("POINT:X_SCREEN").dimensions().size(), 0u);
    TS_ASSERT_EQUALS(rootOut.child(0)->property("POINT:X_SCREEN").dimensions().size(), 0u);
    TS_ASSERT_EQUALS(rootIn.child(0)->property("SUBJECTS:NAMES").dimensions().size(), 1u);
    TS_ASSERT_EQUALS(rootOut.child(0)->property("SUBJECTS:NAMES").dimensions().size(), 1u);
  }

  CXXTEST_TEST(writePoint256)
  {
    // Verify that mutiple groups are create when the number of items is greater that 255.

    // Generate a new trial with more than 256 time sequences
    // Export the trial to a C3D file
    // Load the C3D file
    // Check the properties for POINT:LABELS
    // The number must be equal to 256
    // Check the label of each time sequence
    TS_WARN("TODO");
  }

  CXXTEST_TEST(writeAnalogOnly)
  {
    ma::Node rootIn("rootIn"), rootOut("rootOut");
    ma::Trial foo("foo", &rootIn);
    ma::TimeSequence bar("bar", 1, 100, 1000.0, 10.0, ma::TimeSequence::Analog, "V", 1.0, 0.0, std::array<double,2>{{-10.0, 10.0}}, foo.timeSequences());
    if (!c3dhandlertest_write("", OPENMA_TDD_PATH_OUT("c3d/analog_only.c3d"), &rootIn)) return;
    if (!c3dhandlertest_read("", OPENMA_TDD_PATH_OUT("c3d/analog_only.c3d"), &rootOut)) return;

    TS_ASSERT_EQUALS( rootOut.children().size(), 1u );
    auto tss = rootOut.findChildren<ma::TimeSequence*>();
    TS_ASSERT_EQUALS( tss.size(), 1u );

    const auto range = std::array<double,2>{{-10., 10.}};
    TS_ASSERT_EQUALS( tss[0]->samples(), 100u );
    TS_ASSERT_EQUALS( tss[0]->sampleRate(), 1000.0 );
    TS_ASSERT_EQUALS( tss[0]->range()[0], range[0] );
    TS_ASSERT_EQUALS( tss[0]->range()[1], range[1] );
  }
};

CXXTEST_SUITE_REGISTRATION(C3DWriterTest)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, capability)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, supportedTrialFormats)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, detectNoDevice)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, detectDeviceNotOpen)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, detectFail)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, detectOk)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, queryOkOne)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, queryOkTwo)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, queryOkThree)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, sample01Rewrited)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, sample09Rewrited)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, writePoint256)
CXXTEST_TEST_REGISTRATION(C3DWriterTest, writeAnalogOnly)