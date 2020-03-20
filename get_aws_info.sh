#! /bin/sh

echo "### [iot] list-things ###"
aws iot list-things

echo "### [iot] list-policies ###"
aws iot list-policies

echo "### [iot] list-certificates ###"
aws iot list-certificates
