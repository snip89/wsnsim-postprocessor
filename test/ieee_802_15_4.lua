local function bitStatus(value, bitNumber)
	local status

	value = bit.lshift(value, 8 - bitNumber)
	value = bit.band(value, 255)
	value = bit.rshift(value, 7)

	if(value == 1) then
		status = "true"
	else
		status = "false"
	end

	return status
end

local function checkBit(value, bitNumber)
	value = bit.lshift(value, 8 - bitNumber)
	value = bit.band(value, 255)
	value = bit.rshift(value, 7)

	return value
end

function format(byteArray)
	local result = { }
	local iterator = 1
	local bool
	local isIntraPan = 0

	-- длина сообщения
	local LEN = byteArray[iterator]
	table.insert(result, { name = "LEN", type = "uint8", value = { LEN } } )
	iterator = iterator + 1

	-- проверка валидности длины
	if(#byteArray - 1 ~= LEN) then
		table.insert(result, { error = "Неверное значение LENGHT (значение: " .. LEN .. ", позиция: " .. 0 .. ")" } )
	else

		-- FCF

		-- FCF Frame Type
		local FCF_FrameType = byteArray[iterator]
		FCF_FrameType = bit.lshift(FCF_FrameType, 5)
		FCF_FrameType = bit.band(FCF_FrameType, 255)
		FCF_FrameType = bit.rshift(FCF_FrameType, 5)
		table.insert(result, { name = "FCF_FrameType", type = "uint8", value = { FCF_FrameType } } )

		-- FCF Security Enabled
		bool = checkBit(byteArray[iterator], 3)
		if(bool == 1) then
			table.insert(result, { name = "FCF_SecurityEnabled", type = "string", value = { 0, #"true", "true" } } )
		end

		-- FCF Frame Pending
		bool = checkBit(byteArray[iterator], 4)
		if(bool == 1) then
			table.insert(result, { name = "FCF_FramePending", type = "string", value = { 0, #"true", "true" } } )
		end

		-- FCF Ack Request
		bool = checkBit(byteArray[iterator], 5)
		if(bool == 1) then
			table.insert(result, { name = "FCF_AckRequest", type = "string", value = { 0, #"true", "true" } } )
		end

		-- FCF Intra-PAN
		bool = checkBit(byteArray[iterator], 6)
		if(bool == 1) then
			table.insert(result, { name = "FCF_Intra-PAN", type = "string", value = { 0, #"true", "true" } } )
			isIntraPan = 1
		end

		iterator = iterator + 1

		-- FCF Dest Addr Mode
		local FCF_DestAddrMode = byteArray[iterator]

		FCF_DestAddrMode = bit.rshift(FCF_DestAddrMode, 4)
		FCF_DestAddrMode = bit.band(FCF_DestAddrMode, 255)
		FCF_DestAddrMode = bit.rshift(FCF_DestAddrMode, 6)

		local DestAddrMode

		if(FCF_DestAddrMode == 0) then
			DestAddrMode = "no address"
			table.insert(result, { name = "FCF_DestAddrMode", type = "string", value = { 0, #DestAddrMode, DestAddrMode } } )
		elseif(FCF_DestAddrMode == 2) then
			DestAddrMode = "16 bit address"
			table.insert(result, { name = "FCF_DestAddrMode", type = "string", value = { 0, #DestAddrMode, DestAddrMode } } )
		elseif(FCF_DestAddrMode == 3) then
			DestAddrMode = "64 bit address"
			table.insert(result, { name = "FCF_DestAddrMode", type = "string", value = { 0, #DestAddrMode, DestAddrMode } } )
		end

		-- FCF Source Addr Mode
		local FCF_SourceAddrMode = byteArray[iterator]

		FCF_SourceAddrMode = bit.band(FCF_SourceAddrMode, 255)
		FCF_SourceAddrMode = bit.rshift(FCF_SourceAddrMode, 6)

		local SourceAddrMode

		if(FCF_SourceAddrMode == 0) then
			SourceAddrMode = "no address"
			table.insert(result, { name = "FCF_SourceAddrMode", type = "string", value = { 0, #SourceAddrMode, SourceAddrMode } } )
		elseif(FCF_SourceAddrMode == 2) then
			SourceAddrMode = "16 bit address"
			table.insert(result, { name = "FCF_SourceAddrMode", type = "string", value = { 0, #SourceAddrMode, SourceAddrMode } } )
		elseif(FCF_SourceAddrMode == 3) then
			SourceAddrMode = "64 bit address"
			table.insert(result, { name = "FCF_SourceAddrMode", type = "string", value = { 0, #SourceAddrMode, SourceAddrMode } } )
		end

		iterator = iterator + 1

		-- SequenceNumber
		local SequenceNumber = byteArray[iterator]
		table.insert(result, { name = "SequenceNumber", type = "uint8", value = { SequenceNumber } } )
		iterator = iterator + 1

		-- DestPanID
		if(FCF_DestAddrMode ~= 0) then
			table.insert(result, { name = "DestPanID", type = "ByteArray", value = { 2, byteArray[iterator], byteArray[iterator + 1] } } )
			iterator = iterator + 2
		end

		-- DestAddr
		if(FCF_DestAddrMode == 2) then
			table.insert(result, { name = "DestAddr", type = "ByteArray", value = { 2, byteArray[iterator], byteArray[iterator + 1] } } )
			iterator = iterator + 2
		elseif(FCF_DestAddrMode == 3) then
			table.insert(result, { name = "DestAddr", type = "ByteArray", value = {
				8,
				byteArray[iterator],
				byteArray[iterator + 1],
				byteArray[iterator + 2],
				byteArray[iterator + 3],
				byteArray[iterator + 4],
				byteArray[iterator + 5],
				byteArray[iterator + 6],
				byteArray[iterator + 7] } } )
			iterator = iterator + 8
		end

		-- SourcePanID
		if(FCF_SourceAddrMode ~= 0 and isIntraPan == 0) then
			table.insert(result, { name = "SourcePanID", type = "ByteArray", value = { 2, byteArray[iterator], byteArray[iterator + 1] } } )
			iterator = iterator + 2
		end

		-- SourceAddr
		if(FCF_SourceAddrMode == 2) then
			table.insert(result, { name = "SourceAddr", type = "ByteArray", value = { 2, byteArray[iterator], byteArray[iterator + 1] } } )
			iterator = iterator + 2
		elseif(FCF_SourceAddrMode == 3) then
			table.insert(result, { name = "SourceAddr", type = "ByteArray", value = {
				8,
				byteArray[iterator],
				byteArray[iterator + 1],
				byteArray[iterator + 2],
				byteArray[iterator + 3],
				byteArray[iterator + 4],
				byteArray[iterator + 5],
				byteArray[iterator + 6],
				byteArray[iterator + 7] } } )
			iterator = iterator + 8
		end

		-- FramePayload
		local FramePayload = { }
		local payloadLen = LEN - iterator

		-- table.insert(result, { name = "Debug", type = "uint8", value = { payloadLen } } )

		FramePayload[1] = payloadLen

		for i = 2, payloadLen + 1 do
			FramePayload[i] = byteArray[iterator]
			iterator = iterator + 1
		end

		table.insert(result, { name = "FramePayload", type = "ByteArray", value = FramePayload } )

		-- RSSI
		table.insert(result, { name = "RSSI", type = "int8", value = { byteArray[iterator] } } )
		iterator = iterator + 1

		-- CORR
		local CORR = byteArray[iterator]

		CORR = bit.lshift(CORR, 1)
		CORR = bit.band(CORR, 255)
		CORR = bit.rshift(CORR, 1)

		table.insert(result, { name = "CORR", type = "uint8", value = { CORR } } )

		-- CHECKSUM
		bool = checkBit(byteArray[iterator], 8)
		if(bool == 1) then
			table.insert(result, { name = "CHECKSUM", type = "string", value = { 0, #"true", "true" } } )
		else
			table.insert(result, { name = "CHECKSUM", type = "string", value = { 0, #"false", "falsey" } } )
		end

	end

	return result
end