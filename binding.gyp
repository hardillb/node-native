{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "test.cpp", "addon.cpp" ],
      "include_dirs" : [".", "<!(node -e \"require('nan')\")"]
    }
  ]
}

