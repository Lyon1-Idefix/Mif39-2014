using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FuzzyLogic
{
	[DataContract]
	public sealed class Curve : NamedObject
	{
		[DataMember(Name="Keyframes")]
		private List < int > KeyframesByID { get; set; }
		[IgnoreDataMember]
		public IndirectObjectList<Keyframe> Keyframes {
			get { 
				if (KeyframesByID == null)
					KeyframesByID = new List < int > ();
				return new IndirectObjectList<Keyframe> (this.Referee, KeyframesByID);
			}
		}
		public void Add ( Keyframe key )
		{
			if (KeyframesByID == null)
				KeyframesByID = new List < int > ();
			KeyframesByID.Add (key);
		}
		public override string ToString ()
		{
			return "Curve: " + Name;
		}
	}
}

