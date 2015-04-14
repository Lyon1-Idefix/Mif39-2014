using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

using UnityEditor;

using System;
using System.Collections;
using System.Collections.Generic;

using DesignPatterns;

using FuzzyLogic;

using Conjunction = FuzzyLogicSolver.TNorm;
using Disjunction = FuzzyLogicSolver.SNorm;
using Activation = FuzzyLogicSolver.TNorm;
using Accumulation = FuzzyLogicSolver.SNorm;

[Serializable]
public class Sensor {
	public Camera lightSensor;
	Texture2D sensorMapping = null;
	public double GetOutput () {
		RenderTexture cameraRT = lightSensor.targetTexture;
		if (cameraRT == null)
			return 0.0;
		RenderTexture currentRT = RenderTexture.active;
		RenderTexture.active = cameraRT;
		if ( sensorMapping == null )
			sensorMapping = new Texture2D (1, 1);
		sensorMapping.ReadPixels (new Rect (0, 0, cameraRT.width, cameraRT.height), 0, 0);
		sensorMapping.Apply();
		RenderTexture.active = currentRT;
		Color pixel = sensorMapping.GetPixel (0, 0);
		float gray = pixel.grayscale;
		return (double) gray;
	}
}

[Serializable]
public class Effector {
	public OutputVariable Input { get; set; }
	public void Update () {
		double value = Input.Value;
		if (!double.IsNaN (value)) {
			float v = (float)value;
			OnValueChanged.Invoke (v);
		}
	}
	public OutputEvent OnValueChanged;
	[Serializable]
	public class OutputEvent : UnityEvent<float>
	{
	}
}

public class SimpleDimmerController {
	public FuzzyLogic.Engine theEngine;
	public FuzzyLogicSolver.Engine theSolver;

	//
	void createEngineDescription ()
	{
		theEngine = new FuzzyLogic.Engine ();
		Console.WriteLine ("Create input variable ambient");
		theEngine.Create <FuzzyLogic.InputVariable> (
			"Name", "Ambient", 
			"Minimum", -1.0f, 
			"Maximum", 1.0f,
			"Terms", theEngine.Create<FuzzyLogic.Curve> (
				"Name", "Dark",
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", -1.0f, "Value", 1, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 1.0f, "Value", 0, "InTangent", 0, "OutTangent", 0)
			),
			"Terms", theEngine.Create<FuzzyLogic.Curve> (
				"Name", "Medium",
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", -1.0f, "Value", 0, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 0, "Value", 1, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 1.0f, "Value", 0, "InTangent", 0, "OutTangent", 0)
			),
			"Terms", theEngine.Create<FuzzyLogic.Curve> (
				"Name", "Bright",
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", -1.0f, "Value", 0, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 1.0f, "Value", 1, "InTangent", 0, "OutTangent", 0)
			)
		);
		//
		Console.WriteLine ("Create output variable power");
		theEngine.Create <FuzzyLogic.OutputVariable> (
			"Name", "Power", 
			"Minimum", -1, 
			"Maximum", 1,
			"Terms", theEngine.Create<FuzzyLogic.Curve> (
				"Name", "Lower",
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", -1, "Value", 1, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 0, "InTangent", 0, "OutTangent", 0)
			),
			"Terms", theEngine.Create<FuzzyLogic.Curve> (
				"Name", "Medium",
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", -1, "Value", 0, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 0, "Value", 1, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 0, "InTangent", 0, "OutTangent", 0)
			),
			"Terms", theEngine.Create<FuzzyLogic.Curve> (
				"Name", "Higher",
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", -1, "Value", 0, "InTangent", 0, "OutTangent", 0),
				"Keyframes", theEngine.Create <FuzzyLogic.Keyframe> ("Time", 1, "Value", 1, "InTangent", 0, "OutTangent", 0)
			)
		);
		//
		Console.WriteLine ("Create proposition");
		theEngine.Create <FuzzyLogic.RuleBlock> (
			"Name", "SimpleDimmer",
			"Conclusions", theEngine.Create <FuzzyLogic.Rule> (
			"Antecedent", theEngine.Create <FuzzyLogic.Antecedent> (
			"Expression", theEngine.Create <FuzzyLogic.Proposition> (
			"Variable", theEngine.Inputs ["Ambient"],
			"Term", theEngine.Inputs ["Ambient"] ["Dark"]
		)
		), 
			"Consequent", theEngine.Create <FuzzyLogic.Consequent> (
			"Conclusions", theEngine.Create <FuzzyLogic.Proposition> (
			"Variable", theEngine.Outputs ["Power"], 
			"Term", theEngine.Outputs ["Power"] ["Higher"]
		)
		), 
			"Weight", 1
		),
			"Conclusions", theEngine.Create <FuzzyLogic.Rule> (
			"Antecedent", theEngine.Create <FuzzyLogic.Antecedent> (
			"Expression", theEngine.Create <FuzzyLogic.Proposition> (
			"Variable", theEngine.Inputs ["Ambient"], 
			"Term", theEngine.Inputs ["Ambient"] ["Medium"]
		)
		), 
			"Consequent", theEngine.Create <FuzzyLogic.Consequent> (
			"Conclusions", theEngine.Create <FuzzyLogic.Proposition> (
			"Variable", theEngine.Outputs ["Power"], 
			"Term", theEngine.Outputs ["Power"] ["Medium"]
		)
		), 
			"Weight", 1
		),
			"Conclusions", theEngine.Create <FuzzyLogic.Rule> (
			"Antecedent", theEngine.Create <FuzzyLogic.Antecedent> (
			"Expression", theEngine.Create <FuzzyLogic.Proposition> (
			"Variable", theEngine.Inputs ["Ambient"], 
			"Term", theEngine.Inputs ["Ambient"] ["Bright"]
		)
		), 
			"Consequent", theEngine.Create <FuzzyLogic.Consequent> (
			"Conclusions", theEngine.Create <FuzzyLogic.Proposition> (
			"Variable", theEngine.Outputs ["Power"], 
			"Term", theEngine.Outputs ["Power"] ["Lower"]
		)
		), 
			"Weight", 1
		)
		);
	}
	public SimpleDimmerController ()
	{
		FuzzyLogicSolver.SNormFactory snormFactory = FuzzyLogicSolver.SNormFactory.Get ();
		FuzzyLogicSolver.TNormFactory tnormFactory = FuzzyLogicSolver.TNormFactory.Get ();
		FuzzyLogicSolver.DefuzzyfierFactory defuzzifierFactory = FuzzyLogicSolver.DefuzzyfierFactory.Get ();
		createEngineDescription ();
		theSolver = FuzzyLogicSolver.Converter.Convert (theEngine);
		theSolver.Configure (
			null,
			null,
			tnormFactory.Create ("Minimum"),
			snormFactory.Create ("Maximum"),
			defuzzifierFactory.Create ("Centroid", "5"));
	}
	public void Process () {
		theSolver.Process ();
	}
}

public class SimpleDimmer : MonoBehaviour {
	public float threshold = 0.01f;
	public float oldSensorValue = -1.0f;
	public float currentSensorValue = -1.0f;
	public float requiredValue = 0.5f;
	public bool Enabled = true;
	public Sensor controllerSensor = new Sensor ();
	public Effector controlledLights = new Effector ();
	public SimpleDimmerController controller;
	void Start () {
		controller = new SimpleDimmerController ();
		controlledLights.Input = controller.theEngine.Outputs["Power"];
	}
	void FixedUpdate () {
		if (Enabled) {
			controller.Process ();
			controlledLights.Update ();
		}
	}
	public void sensorValueChanged ( float v ) {
		currentSensorValue = v;
		controller.theEngine.Inputs["Ambient"].Value = v - requiredValue;
		oldSensorValue = v;
	}
	public void requiredLightingChanged (float value) {
		bool oldEnabled = Enabled;
		Enabled = false;
		requiredValue = value;
		Enabled = oldEnabled;
	}
}
