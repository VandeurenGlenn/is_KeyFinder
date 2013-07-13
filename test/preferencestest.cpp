/*************************************************************************

  Copyright 2011-2013 Ibrahim Sha'ath

  This file is part of KeyFinder.

  KeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "preferencestest.h"

void SettingsWrapperFake::beginGroup(const QString& g) { prefix = g; }
void SettingsWrapperFake::endGroup() { prefix.clear(); }
QVariant SettingsWrapperFake::value(const QString &key, const QVariant &defaultValue) const {
  return (hash.contains(prefix + key) ? hash[prefix + key] : defaultValue);
}
void SettingsWrapperFake::setValue(const QString &key, const QVariant &value) {
  hash[prefix + key] = value;
}
QStringList SettingsWrapperFake::allKeys() const {
  return hash.uniqueKeys();
}

TEST (PreferencesTest, ConstantsMetadataFormat) {
  ASSERT_EQ(0, (int) METADATA_FORMAT_KEYS);
  ASSERT_EQ(1, (int) METADATA_FORMAT_CUSTOM);
  ASSERT_EQ(2, (int) METADATA_FORMAT_BOTH);
}

TEST (PreferencesTest, ConstantsMetadataWrite) {
  ASSERT_EQ(0, (int) METADATA_WRITE_NONE);
  ASSERT_EQ(1, (int) METADATA_WRITE_PREPEND);
  ASSERT_EQ(2, (int) METADATA_WRITE_APPEND);
  ASSERT_EQ(3, (int) METADATA_WRITE_OVERWRITE);
}

TEST (PreferencesTest, ConstantsMetadataTag) {
  ASSERT_EQ(0, (int) METADATA_TAG_TITLE);
  ASSERT_EQ(1, (int) METADATA_TAG_ARTIST);
  ASSERT_EQ(2, (int) METADATA_TAG_ALBUM);
  ASSERT_EQ(3, (int) METADATA_TAG_COMMENT);
  ASSERT_EQ(4, (int) METADATA_TAG_GROUPING);
  ASSERT_EQ(5, (int) METADATA_TAG_KEY);
  ASSERT_EQ(6, METADATA_TAG_T_COUNT);
}

TEST (PreferencesTest, ConstructorDefaultsCore) {
  SettingsWrapper* fakeSettings = new SettingsWrapperFake();
  Preferences p(fakeSettings);

  ASSERT_FLOAT_EQ(p.core.getStartingFreqADefault(), p.core.getStartingFreqA());
  ASSERT_EQ(p.core.getOctavesDefault(), p.core.getOctaves());
  ASSERT_EQ(p.core.getOffsetToCDefault(), p.core.getOffsetToC());
  ASSERT_EQ(p.core.getBandsPerOctaveDefault(), p.core.getBandsPerOctave());
  ASSERT_EQ(p.core.getTemporalWindowDefault(), p.core.getTemporalWindow());
  ASSERT_EQ(p.core.getFftFrameSizeDefault(), p.core.getFftFrameSize());
  ASSERT_EQ(p.core.getHopsPerFrameDefault(), p.core.getHopsPerFrame());
  ASSERT_FLOAT_EQ(p.core.getDirectSkStretchDefault(), p.core.getDirectSkStretch());
  ASSERT_EQ(p.core.getTuningMethodDefault(), p.core.getTuningMethod());
  ASSERT_FLOAT_EQ(p.core.getDetunedBandWeightDefault(), p.core.getDetunedBandWeight());
  ASSERT_EQ(p.core.getSegmentationDefault(), p.core.getSegmentation());
  ASSERT_EQ(p.core.getSegGaussianSizeDefault(), p.core.getSegGaussianSize());
  ASSERT_FLOAT_EQ(p.core.getSegGaussianSigmaDefault(), p.core.getSegGaussianSigma());
  ASSERT_EQ(p.core.getSegPeakPickingNeighboursDefault(), p.core.getSegPeakPickingNeighbours());
  ASSERT_EQ(p.core.getArbitrarySegmentsDefault(), p.core.getArbitrarySegments());
  ASSERT_EQ(p.core.getToneProfileDefault(), p.core.getToneProfile());
  ASSERT_EQ(p.core.getSimilarityMeasureDefault(), p.core.getSimilarityMeasure());
  std::vector<float> ctpDefault = p.core.getCustomToneProfileDefault();
  std::vector<float> ctp = p.core.getCustomToneProfile();
  ASSERT_EQ(ctpDefault.size(), ctp.size());
  for (unsigned int i = 0; i < ctp.size(); i++)
    ASSERT_FLOAT_EQ(ctpDefault[i], ctp[i]);
}

TEST (PreferencesTest, ConstructorDefaultsGui) {
  SettingsWrapper* fakeSettings = new SettingsWrapperFake();
  Preferences p(fakeSettings);

  ASSERT_EQ(false, p.getWriteToFilesAutomatically());
  ASSERT_EQ(true, p.getParallelBatchJobs());
  ASSERT_EQ(METADATA_WRITE_NONE, p.getMetadataWriteTitle());
  ASSERT_EQ(METADATA_WRITE_NONE, p.getMetadataWriteArtist());
  ASSERT_EQ(METADATA_WRITE_NONE, p.getMetadataWriteAlbum());
  ASSERT_EQ(METADATA_WRITE_PREPEND, p.getMetadataWriteComment());
  ASSERT_EQ(METADATA_WRITE_NONE, p.getMetadataWriteGrouping());
  ASSERT_EQ(METADATA_WRITE_NONE, p.getMetadataWriteKey());
  ASSERT_EQ(METADATA_WRITE_NONE, p.getMetadataWriteFilename());
  ASSERT_EQ(METADATA_FORMAT_KEYS, p.getMetadataFormat());
  ASSERT_EQ(false, p.getSkipFilesWithExistingTags());
  ASSERT_EQ(60, p.getMaxDuration());
#ifdef Q_OS_WIN
  QString iTunesLibraryPathDefault = QDir::homePath() + "/My Music/iTunes/iTunes Music Library.xml";
  QString traktorLibraryPathDefault = QDir::homePath() + "/My Documents/Native Instruments/Traktor 2.1.2/collection.nml";
  QString seratoLibraryPathDefault = QDir::homePath() + "/My Music/_Serato_/database V2";
#else
  QString iTunesLibraryPathDefault = QDir::homePath() + "/Music/iTunes/iTunes Music Library.xml";
  QString traktorLibraryPathDefault = QDir::homePath() + "/Documents/Native Instruments/Traktor 2.1.2/collection.nml";
  QString seratoLibraryPathDefault = QDir::homePath() + "/Music/_Serato_/database V2";
#endif
  ASSERT_EQ(iTunesLibraryPathDefault, p.getITunesLibraryPath());
  ASSERT_EQ(traktorLibraryPathDefault, p.getTraktorLibraryPath());
  ASSERT_EQ(seratoLibraryPathDefault, p.getSeratoLibraryPath());
  QStringList customKeyCodes = p.getCustomKeyCodes();
  ASSERT_EQ(25, customKeyCodes.length());
  for (int i = 0; i < customKeyCodes.length(); i++)
    ASSERT_EQ(QString(), customKeyCodes[i]);
  ASSERT_EQ(QByteArray(), p.getBatchWindowState());
  ASSERT_EQ(QByteArray(), p.getBatchWindowGeometry());
  ASSERT_EQ(QByteArray(), p.getBatchWindowSplitterState());
}

TEST (PreferencesTest, QSettingsInteraction) {
  SettingsWrapper* fakeSettings = new SettingsWrapperFake();
  Preferences p(fakeSettings);
  p.save();
  ASSERT_FLOAT_EQ(27.5, fakeSettings->value("corestartingFrequencyA", 0).toFloat());
}