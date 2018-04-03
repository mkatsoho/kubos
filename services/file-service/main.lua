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

-- Message format: See README for more details
-- { channel, message, ...args }

local uv = require 'uv'
local wrapRead = require('coro-channel').wrapRead
-- local wrapWrite = require('coro-channel').wrapWrite
local stdin = require('pretty-print').stdin
local stdout = require('pretty-print').stdout
local stderr = require('pretty-print').stderr
local dump = require('pretty-print').dump

function _G.print(...)
  local n = select('#', ...)
  local arguments = {...}
  for i = 1, n do
    arguments[i] = tostring(arguments[i])
  end
  uv.write(stderr, table.concat(arguments, "\t") .. "\n")
end

function _G.p(...)
  local n = select('#', ...)
  local arguments = {...}
  for i = 1, n do
    arguments[i] = dump(arguments[i])
  end
  uv.write(stderr, table.concat(arguments, "\t") .. "\n")
end

local rechunk = require 'rechunk'

local read = rechunk(rechunk(wrapRead(stdin), 1234567), 123456)
-- local write = wrapWrite(stdout)

coroutine.wrap(function ()
  local success, failure = xpcall(function ()
    local i = 0
    for chunk in read do
      i = i + 1
      p({i=i}, {chunk=chunk and #chunk})
      stdout:write(chunk)
    end
    print("DONE")
    -- write()
  end, debug.traceback)
  if success then
    print "Success"
  else
    print "Failure:"
    print(failure)
  end
end)()



-- Index File
-- recursion depth
--

-- upload (path, hash, depth) -- Request that file is uploaded with given hash
-- need (hash, ...)
-- download (hash) - request
-- data (data) -- send data chunk to the other side
-- ack (hash)
-- nak (hash, ...)

uv.run()
