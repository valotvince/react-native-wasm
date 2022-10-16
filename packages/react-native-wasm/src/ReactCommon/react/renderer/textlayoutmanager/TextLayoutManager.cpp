/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "TextLayoutManager.h"

namespace facebook {
namespace react {

void *TextLayoutManager::getNativeTextLayoutManager() const { return (void *)this; }

TextMeasurement TextLayoutManager::measure(
  AttributedStringBox attributedStringBox, ParagraphAttributes paragraphAttributes,
  LayoutConstraints layoutConstraints) const {
  TextMeasurement::Attachments attachments;

  float previousX = 0;
  float previousY = 24;

  for (auto const &fragment : attributedStringBox.getValue().getFragments()) {
    auto textAttributes = fragment.textAttributes;
    auto fontSize = textAttributes.fontSize;

    auto textWidth = fragment.string.size() * fontSize;

    if (fragment.isAttachment()) {
      attachments.push_back(TextMeasurement::Attachment{{{previousX, 0}, {textWidth, fontSize}}, false});
    }

    previousX = textWidth * fontSize;
    previousY = fontSize + 2;
  }

  return TextMeasurement{{previousX, previousY}, attachments};
}

LinesMeasurements TextLayoutManager::measureLines(
  AttributedString attributedString, ParagraphAttributes paragraphAttributes, Size size) const {
  return {};
};

} // namespace react
} // namespace facebook
