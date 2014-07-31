require 'spec_helper'

describe Tweet do
  it { should have_valid(:username).when('theebeastmaster', 'heroiceric') }
  it { should_not have_valid(:username).when(nil, '') }

  it { should have_valid(:message).when('Picard just violated the prime directive, again.', 'Resistance is futile') }
  it { should_not have_valid(:message).when(nil, '') }

  #it { should validate_uniqueness_of(:tweet_id) }

  describe '#as_json' do
    subject(:tweet) { FactoryGirl.create(:tweet) }

    it 'should return only the username and message' do
      expect(tweet.as_json).to eq({ username: "@#{tweet.username}", message: tweet.message })
    end
  end
end
