using System;
using System.Runtime.InteropServices;
using System.Text;

namespace AppUI.HikSDK
{
	public class HikDecoderInterface
	{
		//Error code
		public const int HIK_DECODER_NOERROR = 0;               // no error
		public const int HIK_DECODER_PARA_OVER = 1;             // input parameter is invalid
		public const int HIK_DECODER_ORDER_ERROR = 2;           // The order of the function to be called is error.
		public const int HIK_DECODER_ALLOC_MEMORY_ERROR = 3;    // Allocate memory failed.
		public const int HIK_DECODER_FILE_HDR_UNKNOWN = 4;      // File Hdr unkonwn
		public const int HIK_DECODER_VERSION_INCORRECT = 5;     // The version of decoder and encoder is not adapted.
		public const int HIK_DECODER_SYS_NOT_SUPPORT = 6;       // Sys error is not adapted.
		public const int HIK_DECODER_INIT_VDECODER_ERROR = 7;   // Initialize VDecoder failed.
		public const int HIK_DECODER_INIT_ADECODER_ERROR = 8;   // Initialize ADecoder failed.
		public const int HIK_DECODER_DEC_ERROR = 9;             // Decode data failed.
		public const int HIK_DECODER_BUF_OVER = 10;             // Source buffer over, need to GetOneFrame
		public const int HIK_DECODER_BUF_LACK = 11;             // Source buffer lack, need to InputData
		public const int HIK_DECODER_INVALID_HANDLE = 12;       // Decoder handle is invalid

		//Used by HK_STD_GetOneFrame
		public const int AUDIO_TYPE = 0;       // audio group
		public const int VIDEO_TYPE = 1;       // video group

		[StructLayout(LayoutKind.Sequential)]
		public struct STD_FRAME_INFO
		{
			public uint dwFrameType;        // Frame type
											// 0 - AUDIO_TYPE-(WAVE_FORMAT_PCM)
											// 1 - VIDEO_TYPE-(YUV_FOMAT_YV12)
			public IntPtr pFrameBuffer;     // OutPut Frame Buffer
			public uint dwWidthOrChannels;  // Video: width;        Audio: channels
			public uint dwHeightOrBPS;      // Video: height;	    Audio: bits/sample
			public uint dwFrameRate;        // Video: framerate;    Audio: samples/sec
			public uint dwFrameSize;        // Frame size
			public uint dwFrameNum;         // Frame num
			public uint dwTimeStamp;        // Comparative time stamp in ms
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4, ArraySubType = UnmanagedType.I1)]
			public byte[] dwReserved;       // Reserved
			//[MarshalAs(UnmanagedType.Struct, SizeConst = 32, ArraySubType = UnmanagedType.Struct)]
			public SYSTEMTIME strSysTime;   // AbsoluteTime
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct SYSTEMTIME
		{
			public uint dwYear;
			public uint dwMonth;
			public uint dwDayOfWeek;
			public uint dwDay;
			public uint dwHour;
			public uint dwMinute;
			public uint dwSecond;
			public uint dwMilliseconds;
		}

		/// <summary>
		///     BOOL __stdcall HK_STD_CreateHandle(IN PBYTE pFileHdrBuffer, IN unsigned long dwFileHdrSize, IN unsigned long dwBufferSize, OUT HANDLE &hHandle);
		///     Initialize DycHikSingleThreadDecode SDK.
		/// </summary>
		/// <param name="pFileHdrBuffer">[in] Pointer to the buffer of file header.--指向文件头缓冲区的指针。</param>
		/// <param name="dwFileHdrSize">[in] Size of file header.--文件头的大小。</param>
		/// <param name="dwBufferSize">
		///     [in] The parameter set the size of buffer allocate by the SDK to buffer the data input.
		///     Suggested value for reference:--该参数设置SDK分配的缓冲区大小，建议参考值：
		///     °‹CIF: °‹200KB;
		///     °‹4CIF: °‹1MB;.
		/// </param>
		/// <param name="pHandle">[out] Handle to operate decoder.--处理解码器的句柄。</param>
		/// <returns>
		///     TRUE - Initialize succeed.
		///     FALSE - Initialize failed.
		/// </returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern bool HK_STD_CreateHandle(IntPtr pFileHdrBuffer, uint dwFileHdrSize, uint dwBufferSize, ref IntPtr pHandle);

		/// <summary>
		///     BOOL __stdcall HK_STD_InputData(IN HANDLE hHandle, IN PBYTE pDataBuf, IN unsigned long dwDataBufSize);
		///     Send data to the buffer of SDK , data input cannot exceed dwBufferSize
		///     defined in HK_STD_CreateHandle per time.
		///     将数据发送到SDK的缓冲区中，每次输入的数据不能超过HK_STD_CreateHandle中定义的dwBufferSize。
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <param name="pDataBuf">[in] Pointer to the buffer for data to decode.</param>
		/// <param name="dwBufferSize">[in] Size of data in buffer pointed by pDataBuf.</param>
		/// <returns>
		///     TRUE - Input data succeed.
		///     FALSE - Input data failed. To get extended error information, call HK_STD_GetLastError.
		/// </returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern bool HK_STD_InputData(IntPtr pHandle, IntPtr pDataBuf, uint dwBufferSize);

		/// <summary>
		///     BOOL __stdcall HK_STD_GetOneFrame(IN HANDLE hHandle, OUT STD_FRAME_INFO *pFrameInfo);
		///     Get data decoded frame by frame via structure STD_FRAME_INFO.
		///     通过结构STD_FRAME_INFO获得逐帧解码的数据。
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <param name="pFrameInfo">[out] Pointer to structure STD_FRAME_INFO.</param>
		/// <returns>
		///     TRUE - Get one frame data decoded succeed.
		///     FALSE - No more frame decoded in buffer of decoder.
		/// </returns>
		/// <remarks>
		///     This function should be called incessantly after HK_STD_InputData until it return FALSE.
		///     此函数应在HK_STD_InputData之后不断调用，直到返回FALSE
		/// </remarks>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern bool HK_STD_GetOneFrame(IntPtr pHandle, ref STD_FRAME_INFO pFrameInfo);

		/// <summary>
		///     BOOL __stdcall HK_STD_DestroyHandle(IN HANDLE hHandle);
		///     Release resource of decoder when it is not needed any more.
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <returns>
		///     TRUE - Function succeeds.
		///     FALSE - Function failed. To get extended error information, call HK_STD_GetLastError.
		/// </returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern bool HK_STD_DestroyHandle(IntPtr pHandle);

		/// <summary>
		///     BOOL __stdcall HK_STD_GetLastError(IN HANDLE hHandle, OUT unsigned long &dwErrorCode);
		///     Get last-error code
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <param name="dwErrorCode">[out] The reference will return last-error code if function succeeds.</param>
		/// <returns>
		///     TRUE - Function succeeds.
		///     FALSE - Function failed.
		/// </returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern bool HK_STD_GetLastError(IntPtr pHandle, ref uint dwErrorCode);

		/// <summary>
		///     unsigned long __stdcall HK_STD_SetThrowBFrameNum(IN HANDLE hHandle, IN unsigned long nNum);
		///     Decoder can give up decoding one or both two B frames in a 'PBB' group.
		///     This function can set the number of B frame not to decode.
		///     解码器可以放弃对“PBB”组中两个B帧之一或全部进行解码。
		///     此功能可以设置不解码的B帧数。
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <param name="nNum">[in] Number of B frames to be thrown(not to decode).</param>
		/// <returns>
		///     The function return previous throw-B-frame number.
		/// </returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern uint HK_STD_SetThrowBFrameNum(IntPtr pHandle, uint nNum);

		/// <summary>
		///     BOOL __stdcall HK_STD_SetSkipAudio(IN HANDLE hHandle, IN BOOL bSkip);
		///     This function can skip audio frame data in decode process.
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <param name="bSkip">
		///     [in] Statement of the skip audio:
		///                 TRUE   skip
		///                 FALSE  not skip
		/// </param>
		/// <returns>
		///     TRUE - Function succeeds.
		///     FALSE - Function failed.
		/// </returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern uint HK_STD_SetSkipAudio(IntPtr pHandle, bool bSkip);

		/// <summary>
		///     BOOL __stdcall HK_STD_SetSecretKey(IN HANDLE hHandle, IN long lKeyType, IN char *pSecretKey, IN long lKeyLen);
		///     This function set secretkey.
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <param name="lKeyType">[in] Type of secretkey.</param>
		/// <param name="pSecretKey">[in] Pointer to secretkey</param>
		/// <param name="lKeyLen">[in] Length of secretkey</param>
		/// <returns></returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern uint HK_STD_SetSecretKey(IntPtr pHandle, uint lKeyType, StringBuilder pSecretKey, uint lKeyLen);

		/// <summary>
		///     BOOL __stdcall HK_STD_SetSkipVideo(IN HANDLE hHandle, IN BOOL bSkip);
		///     This function can skip video frame data in decode process.
		/// </summary>
		/// <param name="pHandle">[in] Handle of decoder.</param>
		/// <param name="bSkip">
		///     [in] Statement of the skip audio:
		///                 TRUE   skip
		///                 FALSE  not skip
		/// </param>
		/// <returns>
		///     TRUE - Function succeeds.
		///     FALSE - Function failed.
		/// </returns>
		[DllImport("DycHikSingleThreadDecode.dll", CallingConvention = CallingConvention.StdCall)]
		public static extern uint HK_STD_SetSkipVideo(IntPtr pHandle, bool bSkip);
	}
}
