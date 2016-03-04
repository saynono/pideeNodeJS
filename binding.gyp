{
  "targets": [
    {
      "target_name": "pidee",
      "sources": [ "src/pidee.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}