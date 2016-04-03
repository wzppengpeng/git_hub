#!/bin/bash
echo "remove existed file.."
rm -rf /usr/local/lib/libpredictImage.so

echo "installing..."
cp libpredictImage.so /usr/local/lib/libpredictImage.so
ldconfig

echo "done.."