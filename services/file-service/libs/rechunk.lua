local ffi = require 'ffi'
local copy = ffi.copy
local new = ffi.new

return function (read, chunk_size)
  local buffer = new('uint8_t[?]', chunk_size)
  local filled = 0
  local chunk = nil
  local remaining = 0
  local offset = 0
  return function()
    if not buffer then return end
    while filled < chunk_size do
      local to_fill = chunk_size - filled
      while not chunk do
        local str = read()
        if not str then break end
        remaining = #str
        if remaining > 0 then
          chunk = new('uint8_t[?]', remaining)
          copy(chunk, str, remaining)
        end
      end
      if not chunk then break end
      if remaining > to_fill then
        copy(buffer + filled, chunk + offset, to_fill)
        offset = offset + to_fill
        remaining = remaining - to_fill
        filled = filled + to_fill
      else
        -- If there was no extra, copy all that is left into the buffer.
        copy(buffer + filled, chunk + offset, remaining)
        filled = filled + remaining
        remaining = 0
        chunk = nil
        offset = 0
      end
    end
    if filled > 0 then
      local len = filled
      filled = 0
      return ffi.string(buffer, len)
    end
  end
end
