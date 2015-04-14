using System;
using System.Collections.Generic;

namespace FuzzyLogicSolver
{
	public class Curve : Object
	{
		public Curve ()
		{
		}
		public float Membership (float time)
		{
			//Console.WriteLine ("Computing term " + theCurve.Name + " membership with " + time);
			if ((theCurve.Keyframes == null) || (theCurve.Keyframes.Count == 0))
				return 0;
			//Console.WriteLine ("Term has keyframes between " + theCurve.Keyframes [0].Time + " and " + theCurve.Keyframes [theCurve.Keyframes.Count-1].Time);
			if (time < theCurve.Keyframes [0].Time)
				return theCurve.Keyframes [0].Value;
			//Console.WriteLine ("required time is after " + theCurve.Keyframes [0].Time);
			if (time > theCurve.Keyframes [theCurve.Keyframes.Count-1].Time)
				return theCurve.Keyframes [theCurve.Keyframes.Count-1].Value;
			//Console.WriteLine ("required time is before " + theCurve.Keyframes [theCurve.Keyframes.Count-1].Time);
			int first = 0;
			while (theCurve.Keyframes [first+1].Time < time)
				first++;
			//Console.WriteLine ("Choosen timing keyframe is : " + first);
			FuzzyLogic.Keyframe keyframe0 = theCurve.Keyframes [first];
			FuzzyLogic.Keyframe keyframe1 = theCurve.Keyframes [first+1];
			//Console.WriteLine ("Keyframe: {0} {1} {2} {3}", keyframe0.Time, keyframe0.Value, keyframe0.InTangent, keyframe0.OutTangent);
			//Console.WriteLine ("Keyframe: {0} {1} {2} {3}", keyframe1.Time, keyframe1.Value, keyframe1.InTangent, keyframe1.OutTangent);
			float t = (time - keyframe0.Time) / (keyframe1.Time - keyframe0.Time);
			float dt = keyframe1.Time - keyframe0.Time;
			//Console.WriteLine ("Values: {0} {1}", t, dt);

			float m0 = keyframe0.OutTangent * dt;
			float m1 = keyframe1.InTangent * dt;
			//Console.WriteLine ("Values: {0} {1}", m0, m1);

			float t2 = t * t;
			float t3 = t2 * t;

			float a = 2 * t3 - 3 * t2 + 1;
			float b = t3 - 2 * t2 + t;
			float c = t3 - t2;
			float d = -2 * t3 + 3 * t2;
			//Console.WriteLine ("Values: {0} {1} {2} {3}", a, b, c, d);

			float result = a * keyframe0.Value + b * m0 + c * m1 + d * keyframe1.Value;
			//Console.WriteLine ("Result: {0}", result);
			return result;
		}
		public FuzzyLogic.Curve theCurve { get; set; }
	}
	//
	public class CurveList : List < Curve > {}
}

