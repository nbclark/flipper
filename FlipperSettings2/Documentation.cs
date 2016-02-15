using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;

namespace MobileSRC.GScroll
{
    public class Documentation
    {
        public Documentation()
        {
            this.Features = new List<Feature>();
        }
        public List<Feature> Features
        {
            get;
            set;
        }
        public Feature FindFeature(string name)
        {
            foreach (Feature f in this.Features)
            {
                if (f.Name == name)
                {
                    return f;
                }
            }
            return null;
        }
        public void AddFeature(string name, string title, string description)
        {
            Feature feature = new Feature();
            feature.Name = name;
            feature.Title = title;
            feature.Description = description;
            this.Features.Add(feature);
        }
        public class Feature
        {
            public Feature()
            {
                //
            }
            [XmlElement()]
            public string Name
            {
                get;
                set;
            }
            [XmlElement()]
            public string Title
            {
                get;
                set;
            }
            [XmlElement()]
            public string Description
            {
                get;
                set;
            }
        }
    }
}
