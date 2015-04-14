using System;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;

namespace FuzzyLogicTest
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			FuzzyLogicSolver.SNormFactory snormFactory = FuzzyLogicSolver.SNormFactory.Get ();
			FuzzyLogicSolver.TNormFactory tnormFactory = FuzzyLogicSolver.TNormFactory.Get ();
			FuzzyLogicSolver.DefuzzyfierFactory defuzzifierFactory = FuzzyLogicSolver.DefuzzyfierFactory.Get ();
			FuzzyLogic.Engine engine = new FuzzyLogic.Engine ();
			//
			Console.WriteLine ("Create input variable ambient");
			engine.Create <FuzzyLogic.InputVariable> (
				"Name", "Ambient", 
				"Minimum", 0, 
				"Maximum", 1,
				"Terms", engine.Create<FuzzyLogic.Curve> (
					"Name", "Dark",
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 0, "Value", 1, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 0, "InTangent", 0, "OutTangent", 0)
				),
				"Terms", engine.Create<FuzzyLogic.Curve> (
					"Name", "Medium",
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 0, "Value", 0, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 0.5f, "Value", 1, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 0, "InTangent", 0, "OutTangent", 0)
				),
				"Terms", engine.Create<FuzzyLogic.Curve> (
					"Name", "Bright",
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 0, "Value", 0, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 1, "InTangent", 0, "OutTangent", 0)
				)
			);
			//
			Console.WriteLine ("Create output variable power");
			engine.Create <FuzzyLogic.OutputVariable> (
				"Name", "Power", 
				"Minimum", -1, 
				"Maximum", 1,
				"Terms", engine.Create<FuzzyLogic.Curve> (
					"Name", "Lower",
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", -1, "Value", 1, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 0, "InTangent", 0, "OutTangent", 0)
				),
				"Terms", engine.Create<FuzzyLogic.Curve> (
					"Name", "Medium",
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", -1, "Value", 0, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 0, "Value", 1, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 0, "InTangent", 0, "OutTangent", 0)
				),
				"Terms", engine.Create<FuzzyLogic.Curve> (
					"Name", "Higher",
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", -1, "Value", 0, "InTangent", 0, "OutTangent", 0),
					"Keyframes", engine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 1, "InTangent", 0, "OutTangent", 0)
				)
			);
			//
			Console.WriteLine ("Create proposition");
			engine.Create <FuzzyLogic.RuleBlock> (
				"Name", "SimpleDimmer",
				"Conclusions", engine.Create <FuzzyLogic.Rule> (
					"Antecedent", engine.Create <FuzzyLogic.Antecedent> (
						"Expression", engine.Create <FuzzyLogic.Proposition> (
							"Variable", engine.Inputs["Ambient"],
							"Term", engine.Inputs["Ambient"]["Dark"]
						)
					), 
					"Consequent", engine.Create <FuzzyLogic.Consequent> (
						"Conclusions", engine.Create <FuzzyLogic.Proposition> (
							"Variable", engine.Outputs["Power"], 
							"Term", engine.Outputs["Power"]["Higher"]
						)
					), 
					"Weight", 1
				),
				"Conclusions", engine.Create <FuzzyLogic.Rule> (
					"Antecedent", engine.Create <FuzzyLogic.Antecedent> (
						"Expression", engine.Create <FuzzyLogic.Proposition> (
							"Variable", engine.Inputs["Ambient"], 
							"Term", engine.Inputs["Ambient"]["Medium"]
						)
					), 
					"Consequent", engine.Create <FuzzyLogic.Consequent> (
						"Conclusions", engine.Create <FuzzyLogic.Proposition> (
							"Variable", engine.Outputs["Power"], 
							"Term", engine.Outputs["Power"]["Medium"]
						)
					), 
					"Weight", 1
				),
				"Conclusions", engine.Create <FuzzyLogic.Rule> (
					"Antecedent", engine.Create <FuzzyLogic.Antecedent> (
						"Expression", engine.Create <FuzzyLogic.Proposition> (
							"Variable", engine.Inputs["Ambient"], 
							"Term", engine.Inputs["Ambient"]["Bright"]
						)
					), 
					"Consequent", engine.Create <FuzzyLogic.Consequent> (
						"Conclusions", engine.Create <FuzzyLogic.Proposition> (
							"Variable", engine.Outputs["Power"], 
							"Term", engine.Outputs["Power"]["Lower"]
						)
					), 
					"Weight", 1
				)
			);

			engine.SerializeMe("myFileName.xml");

			Console.WriteLine ("Create solver, configure and process");
			Console.WriteLine ("#Objects: " + FuzzyLogic.Object.currentID);
			Console.WriteLine ("#inputs: " + engine.Inputs.Count + " #outputs: " + engine.Outputs.Count + " #blocks: " + engine.Blocks.Count);

			/**/
			FuzzyLogicSolver.Engine theEngine = FuzzyLogicSolver.Converter.Convert (engine);

			engine.Inputs["Ambient"].Value = 0.25f;

			theEngine.Configure (
				null,
				null,
				tnormFactory.Create ("Minimum"),
				snormFactory.Create ("Maximum"),
				defuzzifierFactory.Create ("Centroid", "5"));
			//
			theEngine.Process ();

			//
			Console.WriteLine ("Input was: " + engine.Inputs["Ambient"].Value);
			Console.WriteLine ("Output is: " + engine.Outputs["Power"].Value);

			/**/
		}
	}
}
