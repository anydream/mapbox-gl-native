#import "MGLFeature.h"
#import "MGLShape.h"

#import <mbgl/util/geo.hpp>
#import <mbgl/util/feature.hpp>
#import <mbgl/style/conversion/geojson.hpp>

NS_ASSUME_NONNULL_BEGIN

/**
 Returns an array of `MGLFeature` objects converted from the given vector of
 vector tile features.
 */
NS_ARRAY_OF(MGLShape <MGLFeature> *) *MGLFeaturesFromMBGLFeatures(const std::vector<mbgl::Feature> &features);

/**
 Returns an `MGLFeature` object converted from the given mbgl::Feature
 */
id <MGLFeature> MGLFeatureFromMBGLFeature(const mbgl::Feature &feature);

/**
 Returns an `MGLShape` representing the given geojson. The shape can be
 a feature, a collection of features, or a geometry.
 */
MGLShape* MGLShapeFromGeoJSON(const mapbox::geojson::geojson &geojson);

/**
 Takes an `mbgl::Feature` object, an identifer, and attributes dictionary and
 returns the feature object with converted `mbgl::FeatureIdentifier` and 
 `mbgl::PropertyMap` properties.
 */
mbgl::Feature mbglFeature(mbgl::Feature feature, id identifier, NSDictionary *attributes);

/**
 Returns an `NSDictionary` representation of an `MGLFeature`.
 */
NS_DICTIONARY_OF(NSString *, id) *NSDictionaryFeatureForGeometry(NSDictionary *geometry, NSDictionary *attributes, id identifier);

NS_ASSUME_NONNULL_END

#define MGLFeatureInitCoding() \
- (instancetype)initWithCoder:(NSCoder *)decoder { \
if (self = [super initWithCoder:decoder]) { \
    NSSet<Class> *identifierClasses = [NSSet setWithArray:@[[NSString class], [NSNumber class]]]; \
    identifier = [decoder decodeObjectOfClasses:identifierClasses forKey:@"identifier"]; \
    attributes = [decoder decodeObjectOfClass:[NSDictionary class] forKey:@"attributes"]; \
} \
return self; \
}

#define MGLFeatureEncode() \
- (void)encodeWithCoder:(NSCoder *)coder { \
[super encodeWithCoder:coder]; \
[coder encodeObject:identifier forKey:@"identifier"]; \
[coder encodeObject:attributes forKey:@"attributes"]; \
}

#define MGLFeatureIsEqual(klass) \
- (BOOL)isEqual:(id)other { \
if (other == self) return YES; \
if (![other isKindOfClass:[self class]]) return NO; \
klass *otherFeature = other; \
return [super isEqual:other] && [self geoJSONObject] == [otherFeature geoJSONObject]; \
} \
- (NSUInteger)hash { \
return [super hash] + [[self geoJSONDictionary] hash]; \
}
