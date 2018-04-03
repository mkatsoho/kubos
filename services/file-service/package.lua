--[[
Copyright (C) 2018 Kubos Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
]]
return {
  name = "kubos/kubos-file-service",
  version = "0.0.1",
  description = "Service for reliable and resumable upload/download of large files.",
  tags = { "kubos", "udp", "file"},
  author = { name = "Tim Caswell", email = "tim@kubos.co" },
  homepage = "https://github.com/kubos/kubos",
  luvi = {
    flavor = "tiny",
    inline = "#!/home/system/usr/bin/luvi-tiny --\n"
  },
  dependencies = {
    "luvit/require",
    "luvit/pretty-print",
    "creationix/cbor",
    "creationix/coro-channel",
  },
  files = {
    "**.lua",
  },
  license = "Apache 2.0"
}
