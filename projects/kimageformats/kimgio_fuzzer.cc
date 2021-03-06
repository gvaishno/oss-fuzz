/*
# Copyright 2018 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################
*/

/*
  Usage:
    python infra/helper.py build_image kimageformats
    python infra/helper.py build_fuzzers --sanitizer undefined|address|memory kimageformats
    python infra/helper.py run_fuzzer kimageformats kimgio_fuzzer
*/


#include <QBuffer>
#include <QCoreApplication>
#include <QImage>

#include "pcx_p.h"
#include "pic_p.h"
#include "psd_p.h"
#include "ras_p.h"
#include "rgb_p.h"
#include "tga_p.h"
#include "xcf_p.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    int argc = 0;
    QCoreApplication a(argc, nullptr);

    const QVector<QImageIOHandler*> handlers = {
        new PCXHandler(),
        new SoftimagePICHandler(),
        new PSDHandler(),
        new RASHandler(),
        new RGBHandler(),
        new TGAHandler(),
        new XCFHandler()
    };

    for (QImageIOHandler *h : handlers) {
        QImage i;
        QBuffer b;
        b.setData((const char *)data, size);
        b.open(QIODevice::ReadOnly);
        h->setDevice(&b);
        h->read(&i);
    }
    qDeleteAll(handlers);

    return 0;
}
