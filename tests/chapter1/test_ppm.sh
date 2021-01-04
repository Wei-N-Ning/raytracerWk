#!/usr/bin/env bash

set -e

TEMPDIR=/tmp/sut

tearDown() {
  rm -rf ${TEMPDIR} /tmp/_ /tmp/_.* /tmp/__*
}

setUp() {
  tearDown
  mkdir -p ${TEMPDIR}
}

sutbin=${1:?missing program}

setUp
${sutbin} >${TEMPDIR}/test.ppm
