#!/bin/bash

while read line; do
    echo "\" $line \\r\\n\""
done < agsimgui.ash
