using System.Runtime.Serialization;

namespace FuzzyLogic
{
	[DataContract]
	public sealed class Keyframe : Object
	{
		[DataMember(Name="Time")]
		public float Time { get; set; }
		[DataMember(Name="Value")]
		public float Value { get; set; }
		[DataMember(Name="InTangent")]
		public float InTangent { get; set; }
		[DataMember(Name="OutTangent")]
		public float OutTangent { get; set; }
	}
}

